#include <kernel/datetime.h>
#include <kernel/util.h>

// Change this each year as needed.
// This is only used if the century register doesn't exist.
#define CURRENT_YEAR 2021
 
unsigned char datetime_second;
unsigned char datetime_minute;
unsigned char datetime_hour;
unsigned char datetime_day;
unsigned char datetime_month;
unsigned int datetime_year;

 // Set by ACPI table parsing code if possible.
int century_register = 0x00;
 
enum {
    cmos_address = 0x70,
    cmos_data    = 0x71
};
 
int get_update_in_progress_flag() {
    outportb(cmos_address, 0x0A);
    return (inportb(cmos_data) & 0x80);
}
 
unsigned char get_RTC_register(int reg) {
      outportb(cmos_address, reg);
      return inportb(cmos_data);
}
 
void datetime_read_rtc() {
    unsigned char century = 0;
    unsigned char last_second;
    unsigned char last_minute;
    unsigned char last_hour;
    unsigned char last_day;
    unsigned char last_month;
    unsigned char last_year;
    unsigned char last_century;
    unsigned char registerB;

    // Note: This uses the "read registers until you get the same values twice in a row" technique
    //       to avoid getting dodgy/inconsistent values due to RTC updates

    while (get_update_in_progress_flag()); // Make sure an update isn't in progress
    datetime_second = get_RTC_register(0x00);
    datetime_minute = get_RTC_register(0x02);
    datetime_hour = get_RTC_register(0x04);
    datetime_day = get_RTC_register(0x07);
    datetime_month = get_RTC_register(0x08);
    datetime_year = get_RTC_register(0x09);
    if (century_register != 0) {
        century = get_RTC_register(century_register);
    }

    do {
        last_second = datetime_second;
        last_minute = datetime_minute;
        last_hour = datetime_hour;
        last_day = datetime_day;
        last_month = datetime_month;
        last_year = datetime_year;
        last_century = century;

        while (get_update_in_progress_flag()); // Make sure an update isn't in progress
        datetime_second = get_RTC_register(0x00);
        datetime_minute = get_RTC_register(0x02);
        datetime_hour = get_RTC_register(0x04);
        datetime_day = get_RTC_register(0x07);
        datetime_month = get_RTC_register(0x08);
        datetime_year = get_RTC_register(0x09);
        if (century_register != 0) {
            century = get_RTC_register(century_register);
        }
    } while ((last_second != datetime_second) || (last_minute != datetime_minute) || (last_hour != datetime_hour) ||
             (last_day != datetime_day) || (last_month != datetime_month) || (last_year != datetime_year) ||
             (last_century != century));

    registerB = get_RTC_register(0x0B);

    // Convert BCD to binary values if necessary

    if (!(registerB & 0x04)) {
        datetime_second = (datetime_second & 0x0F) + ((datetime_second / 16) * 10);
        datetime_minute = (datetime_minute & 0x0F) + ((datetime_minute / 16) * 10);
        datetime_hour = ((datetime_hour & 0x0F) + (((datetime_hour & 0x70) / 16) * 10)) | (datetime_hour & 0x80);
        datetime_day = (datetime_day & 0x0F) + ((datetime_day / 16) * 10);
        datetime_month = (datetime_month & 0x0F) + ((datetime_month / 16) * 10);
        datetime_year = (datetime_year & 0x0F) + ((datetime_year / 16) * 10);
        if (century_register != 0) {
            century = (century & 0x0F) + ((century / 16) * 10);
        }
    }

    // Convert 12 hour clock to 24 hour clock if necessary
    if (!(registerB & 0x02) && (datetime_hour & 0x80)) {
        datetime_hour = ((datetime_hour & 0x7F) + 12) % 24;
    }

    // Calculate the full (4-digit) year
    if (century_register != 0) {
        datetime_year += century * 100;
    } else {
        datetime_year += (CURRENT_YEAR / 100) * 100;
        if (datetime_year < CURRENT_YEAR) {
            datetime_year += 100;
        }
    }
}
