#define ET6I_C

/****************** Includes ********************/

#include "et6i.h"
#include "nrf24l01.h"

#include <stdio.h>
#include <string.h>

/******************* Defines ********************/

/******************** Types *********************/

/***************** Private Data *****************/

/****************** Prototypes ******************/

/**************** Implementation ****************/

// Initialize E-Sky ET6I RC receiver driver for nRF24L01
uint8_t et6i_init( void )
{
  // Initialize privates
  memset( &et6i_s.prv_s, 0, sizeof(et6i_s.prv_s) );

  // Initialize outputs
  memset( &et6i_s.outp_s, 0, sizeof(et6i_s.outp_s) );

  // Done
  return 0;
} 

// nRF24L01 runtime handling
void et6i( void )
{
  uint8_t retVal_u8;
  uint8_t dat_au8[13];

  // State machine
  switch( et6i_s.prv_s.st_e )
  {
  case et6i_stMacInit_E: // Prepare for reception of binding packet

    // Configure transceiver
    et6i_s.prv_s.cfgTrx_s.ch_u8          = 50;
    et6i_s.prv_s.cfgTrx_s.dataRate_e     = nrf24l01_dataRate1Mbps_E;
    et6i_s.prv_s.cfgTrx_s.flgAutoAck_u8  = 0;
    et6i_s.prv_s.cfgTrx_s.crcSize_u8     = 2;
    et6i_s.prv_s.cfgTrx_s.payloadsize_u8 = 13;
    et6i_s.prv_s.cfgTrx_s.addrSize_u8    = 3;
    et6i_s.prv_s.cfgTrx_s.addr_u64       = 0x000000;
    nrf24l01_cfgStart( &et6i_s.prv_s.cfgTrx_s );

    // Go to next state
    et6i_s.prv_s.st_e = et6i_stMacWaitForConfig_E;

    break;

  case et6i_stMacWaitForConfig_E: // Wait until TRX configuration is complete

    // Check if configuration finished
    if( !nrf24l01_cfgBusy() )
    {
      // Go to next state
      et6i_s.prv_s.st_e = et6i_stMacWaitForBinding_E;

      // Debug
      printf("Wait for TX signal ... ");
    }
    break;

  case et6i_stMacWaitForBinding_E: // Receive binding packets from TX

    // Check and get new data from RX FIFO
    retVal_u8 = nrf24l01_getRxData( dat_au8, sizeof(dat_au8) );

    // If there is data, perform plausibility check message content
    if( retVal_u8 == 0 )
    {
      // Check band modulation scheme and constants in message
      if( (dat_au8[3] < 10) || (dat_au8[3] > 83) )
      {
        // No correct band
        retVal_u8 = 1;
      }
      else if ( (dat_au8[ 4] != 0x18) ||
                (dat_au8[ 5] != 0x29) ||
                (dat_au8[ 6] != 0x00) ||
                (dat_au8[ 7] != 0x00) ||
                (dat_au8[ 8] != 0x00) ||
                (dat_au8[ 9] != 0x00) ||
                (dat_au8[10] != 0x00) ||
                (dat_au8[11] != 0x00) ||
                (dat_au8[12] != 0x00) )
      {
        // No correct constants
        retVal_u8 = 1;
      }
    }

    // Only process messages with correct format
    if( retVal_u8 == 0)
    {
      // Indicate binding has started
      et6i_s.outp_s.flgBind_u8 = 1;

      // On first packet record address data and band info
      if( et6i_s.prv_s.bandInfo_u8 == 0 )
      {
        // Store address
        et6i_s.prv_s.addr_au8[0] = dat_au8[0];
        et6i_s.prv_s.addr_au8[1] = dat_au8[1];
        et6i_s.prv_s.addr_au8[2] = dat_au8[2];

        // Store band information
        et6i_s.prv_s.bandInfo_u8 = dat_au8[3];

        // Debug info
        printf("ok\n");
        printf("Bind ... ");
      }
      else // On all other binding packets compare for identical values
      {
      if( (et6i_s.prv_s.addr_au8[0] != dat_au8[0]) ||
          (et6i_s.prv_s.addr_au8[1] != dat_au8[1]) ||
          (et6i_s.prv_s.addr_au8[2] != dat_au8[2]) ||
          (et6i_s.prv_s.bandInfo_u8 != dat_au8[3]) )
      {
        et6i_s.prv_s.st_e = et6i_stMacBindingAbort_E;

        // Debug info
        printf("Abort... wrong binding repetition detected\n");
        printf("Addr/Info %02X:%02X:%02X:%02X != %02X:%02X:%02X:%02X\n",
           et6i_s.prv_s.addr_au8[0],
           et6i_s.prv_s.addr_au8[1],
           et6i_s.prv_s.addr_au8[2],
           et6i_s.prv_s.addr_au8[3],
           dat_au8[0],
           dat_au8[1],
           dat_au8[2],
           dat_au8[3] );
      }
      else
      {
        et6i_s.prv_s.cnt_u32++;
      }

      // If we received 200 consecutive frames without any other
      // transmitter sending binding information we consider binding success
      if( et6i_s.prv_s.cnt_u32 >= 200 )
      {
        printf( "ok\n" );

        // Transmitter will transmit on two bands.
        // Each band is 7 channels wide.
        // Data will be on center channel.
        // The two data channels are 37 channels apart.
        // The band information byte indicates the start channels.
        // The transmitter will increase band info value by one on each start.
        // This shifts band 5 channels up.
        // In case one of the bands reaches 80 some special handling is done (see below).

        // Determine band 1 and 2 start channel
        uint16_t band1_u16 = 7;
        uint16_t band2_u16 = band1_u16 + 37;
        for( uint16_t idx_u16 = 10; idx_u16 < 84; idx_u16++ )
        {
          // Stop if index is equal to band info byte
          if( et6i_s.prv_s.bandInfo_u8 == idx_u16 )
          {
            break;
          }

          // Go to next band start channel (5 channels further)
          band1_u16 += 5;
          band2_u16 += 5;

          // On band start channel above 80, use new start channel that
          // is 37 smaller than the current other band start channel
          if( band2_u16 > 80 )
          {
            band2_u16 = band1_u16 - 37;
          }
          if( band1_u16 > 80 )
          {
            band1_u16 = band2_u16 - 37;
          }
        }
        et6i_s.prv_s.strtBand1_u8 = band1_u16;
        et6i_s.prv_s.strtBand2_u8 = band2_u16;
        et6i_s.prv_s.cnt_u32      = 0;

        // Go to next state
        printf( "Scanning for payload ... " );
        et6i_s.prv_s.st_e = et6i_stMacConfigRxData_E;

        }
      }
    }

    break;

  case et6i_stMacConfigRxData_E: // Reconfigure transceiver for data reception

    // Set new 4-byte address as received during binding
    et6i_s.prv_s.cfgTrx_s.addr_u64  = 0;
    et6i_s.prv_s.cfgTrx_s.addr_u64 |= (uint64_t)et6i_s.prv_s.addr_au8[2] <<  0;
    et6i_s.prv_s.cfgTrx_s.addr_u64 |= (uint64_t)et6i_s.prv_s.addr_au8[1] <<  8;
    et6i_s.prv_s.cfgTrx_s.addr_u64 |= (uint64_t)et6i_s.prv_s.addr_au8[0] << 16;
    et6i_s.prv_s.cfgTrx_s.addr_u64 |= (uint64_t)0xBB                     << 24;
    et6i_s.prv_s.cfgTrx_s.addrSize_u8 = 4;

    // Start searching for payload on center of band 1
    et6i_s.prv_s.currCh1_u8 = et6i_s.prv_s.strtBand1_u8 + 3;
    et6i_s.prv_s.currCh2_u8 = et6i_s.prv_s.strtBand2_u8 + 3;
    et6i_s.prv_s.cfgTrx_s.ch_u8 = et6i_s.prv_s.currCh1_u8;

    // Reconfigure device
    nrf24l01_cfgStart( &et6i_s.prv_s.cfgTrx_s );

    // Wait until finished
    et6i_s.prv_s.st_e = et6i_stMacWaitForReconfig_E;

    break;

  case et6i_stMacWaitForReconfig_E: // Wait until reconfiguration is complete

    // Check if configuration finished
    if( !nrf24l01_cfgBusy() )
    {
      // Check first channel for data
      et6i_s.prv_s.st_e = et6i_stMacScanForRxData_E;
    }
    break;

  case et6i_stMacScanForRxData_E: // Wait until RX data is received on first channel

    retVal_u8 = nrf24l01_getRxData( dat_au8, sizeof(dat_au8) );
    if( retVal_u8 == 0 )
    {
      // Plausibility check on last byte
      if( (dat_au8[12] == 6                            ) ||
          (dat_au8[12] == et6i_s.prv_s.currCh1_u8 * 2) ||
          (dat_au8[12] == et6i_s.prv_s.currCh2_u8 * 2) )
      {
        // Value is as expected. Increment count.
        et6i_s.prv_s.cnt_u32++;

        // In case we received 20 correct packets we are operational
        if( et6i_s.prv_s.cnt_u32 >= 20 )
        {
          printf( "ok\n" );

          // Operational
          et6i_s.prv_s.st_e        = et6i_stMacRx_E;
          et6i_s.prv_s.cnt_u32     = 0;
          et6i_s.outp_s.flgCon_u8  = 1;
          et6i_s.outp_s.flgBind_u8 = 0;
        }
      }
    }

    break;

  case et6i_stMacRx_E: // Receive and decode channel data

    // Get data
    retVal_u8 = nrf24l01_getRxData( dat_au8, sizeof(dat_au8) );
    if( retVal_u8 == 0 )
    {
      // Plausibility check on last byte
      if( (dat_au8[12] == 6                            ) ||
          (dat_au8[12] == et6i_s.prv_s.currCh1_u8 * 2) ||
          (dat_au8[12] == et6i_s.prv_s.currCh2_u8 * 2) )
      {
        // Extract data from packet
        et6i_s.outp_s.ch1_us_u16 = (uint16_t)dat_au8[ 0] << 8 | dat_au8[ 1];
        et6i_s.outp_s.ch2_us_u16 = (uint16_t)dat_au8[ 2] << 8 | dat_au8[ 3];
        et6i_s.outp_s.ch3_us_u16 = (uint16_t)dat_au8[ 4] << 8 | dat_au8[ 5];
        et6i_s.outp_s.ch4_us_u16 = (uint16_t)dat_au8[ 6] << 8 | dat_au8[ 7];
        et6i_s.outp_s.ch5_us_u16 = (uint16_t)dat_au8[ 8] << 8 | dat_au8[ 9];
        et6i_s.outp_s.ch6_us_u16 = (uint16_t)dat_au8[10] << 8 | dat_au8[11];

        // Reset timeout count
        et6i_s.prv_s.cntTimeout_u32 = 0;

        // Increment packet count (for active channel)
        if( et6i_s.prv_s.cfgTrx_s.ch_u8 == et6i_s.prv_s.currCh1_u8 )
        {
          et6i_s.prv_s.cntPktCh1_u32++;
        }
        else
        {
          et6i_s.prv_s.cntPktCh2_u32++;
        }
      }
    }

    // Check if we should switch channels (1/4th of timeout time)
    et6i_s.prv_s.cnt_u32 += et6i_s.prm_s.tiCyc_us_u16;
    if( et6i_s.prv_s.cnt_u32 >= (uint32_t)et6i_s.prm_s.timeoutRx_ms_u8 * 1000 / 4 )
    {
      // Reset channel switch count
      et6i_s.prv_s.cnt_u32 = 0;

      // Check which channel is active and select the other one
      if( et6i_s.prv_s.cfgTrx_s.ch_u8 == et6i_s.prv_s.currCh1_u8 )
      {
        et6i_s.prv_s.cfgTrx_s.ch_u8 = et6i_s.prv_s.currCh2_u8;
      }
      else
      {
        et6i_s.prv_s.cfgTrx_s.ch_u8 = et6i_s.prv_s.currCh1_u8;
      }

      // Set new channel
      nrf24l01_switchChannel( et6i_s.prv_s.cfgTrx_s.ch_u8 );
    }

    // Check if we have a timeout
    if( et6i_s.prv_s.cntTimeout_u32 >= (uint32_t)et6i_s.prm_s.timeoutRx_ms_u8 * 1000 )
    {
      // Connection lost
      et6i_s.outp_s.flgLost_u8 = 1;
    }
    else
    {
      // Connection restored
      et6i_s.prv_s.cntTimeout_u32 += et6i_s.prm_s.tiCyc_us_u16;
      et6i_s.outp_s.flgLost_u8 = 0;
    }

    break;

  default:
    // Do nothing
    break;

  }
}

// EOF

