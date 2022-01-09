
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"
#include "usb_descriptors.h"


static bool button_state = false;
void led_blinking_task(void);
void hid_task(void);
const int gpioports[] = {10,11,12,13,14,15}; // Only for standard keys
bool buttonstates[] ={0,0,0,0};
const int size = 3;


   bool conkey_pressed[] = {0,0};

void ctrl_task()
{
   const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;
  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;
  int index = 0;
  uint16_t volume;
  int pressedport=0;
  for(int i=4;i<6;i++)
  {
    index = i - 4;
  if ( gpio_get(gpioports[i]))
      {
        pressedport = gpioports[i];
        if (tud_suspended()) { //Wakeup if we are sleeping
                tud_remote_wakeup();
            }
              if(!conkey_pressed[index])
              {
                if(pressedport == 14)
                {
                  volume = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
                  tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume, 2);
                  conkey_pressed[index] = 1;
                  board_led_write(1); 
                }
                if(pressedport == 15)
                {
                  volume = HID_USAGE_CONSUMER_VOLUME_INCREMENT;
                  tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume, 2);
                  conkey_pressed[index] = 1;
                  board_led_write(1); 
                }
              }
      }
      else
      {
        // send empty key report (release key) if previously has key pressed
        uint16_t empty_key = 0;
        if (conkey_pressed[index]){
            if (tud_hid_ready()) {
         tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
                  conkey_pressed[index] = false;
                  board_led_write(0);
        }
        }
      } // else
  } // for loop
  

}



int main(void)
{ 
  
    stdio_init_all();
    for(int i=0;i<3;i++)
    {
      sleep_ms(20);
      board_led_write(1);
      sleep_ms(20);
            board_led_write(0);
      gpio_pull_down(gpioports[i]);
    }
    gpio_pull_down(14);
    gpio_pull_down(15);

  board_init();
  tusb_init();

  while (1)
  {
    hid_task();
    ctrl_task();
    tud_task(); // tinyusb device task
  }
  return 0;
}
 
    

// Invoked when device is mounted
void tud_mount_cb(void)
{
}
// Invoked when device is unmounted
void tud_umount_cb(void)
{
}
// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
}
// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
}


uint8_t gpioportsToKeycode(int index)
{
  switch(index)
  {
    case 10: return HID_KEY_A;break;
    case 11: return HID_KEY_W;break;
    case 12: return HID_KEY_S;break;
    case 13: return HID_KEY_D;break;
    default:;break;
  }
}


/* This is a basic implementation of changing audio
case REPORT_ID_CONSUMER_CONTROL:
    {
      // use to avoid send multiple consecutive zero report
      static bool has_consumer_key = false;
      if ( btn )
      {
        // volume down
        uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
        has_consumer_key = true;
      }else
      {
        // send empty key report (release key) if previously has key pressed
        uint16_t empty_key = 0;
        if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
        has_consumer_key = false;
      }
    }
    break;
*/
// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;
  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;
 for(int i=0;i<4;i++) // go through keys
    {
      if (gpio_get(gpioports[i])) { //If the button is pressed
            if (tud_suspended()) { //Wakeup if we are sleeping
                tud_remote_wakeup();
            }
            if (!buttonstates[i]) { //If the button state was previously different
                printf("DEBUG: Button pressed!\n" );
                     board_led_write(1);
               if (tud_hid_ready()) {
                   uint8_t keycode[6] = {0,0,0,0,0,0};
                   keycode[0] = gpioportsToKeycode(gpioports[i]);
                   tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0 , keycode);
                   buttonstates[i] = 1;
                   }
               }
           } 
            else { //If the button is depressed
              if (buttonstates[i]) { //If the button state was previously different
               if (tud_hid_ready()) {
                   board_led_write(0);
                   uint8_t keycode[6] = {0,0,0,0,0,0};
                   tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode); //Release Key
                   buttonstates[i] = 0;
               }
           }           
        }
    }  
    }




// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;
    }
  }
}


