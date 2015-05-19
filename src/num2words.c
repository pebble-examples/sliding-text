#include "num2words.h"

#include <string.h>
#include <stdio.h>

static const char* const ONES[] = {
  "zero",
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine"
};

static const char* const TEENS[] ={
  "",
  "eleven",
  "twelve",
  "thirteen",
  "fourteen",
  "fifteen",
  "sixteen",
  "seventeen",
  "eighteen",
  "nineteen"
};

static const char* const TEENS_SPLIT[][2] = {
  {"", ""},
  {"eleven",""},
  {"twelve",""},
  {"thirteen",""},
  {"four","teen"},
  {"fifteen",""},
  {"sixteen",""},
  {"seven","teen"},
  {"eight","teen"},
  {"nine","teen"}
};

static const char* const TENS[] = {
  "",
  "ten",
  "twenty",
  "thirty",
  "forty",
  "fifty",
  "sixty",
  "seventy",
  "eighty",
  "ninety"
};

static const char* STR_OH_TICK = "o'";
static const char* STR_CLOCK = "clock";
static const char* STR_NOON = "noon";
static const char* STR_MIDNIGHT = "midnight";
static const char* STR_QUARTER = "quarter";
static const char* STR_TO = "to";
static const char* STR_PAST = "past";
static const char* STR_HALF = "half";
static const char* STR_AFTER = "after";

static size_t append_number(char* words, int num) {
  int tens_val = num / 10 % 10;
  int ones_val = num % 10;

  size_t len = 0;

  if (tens_val > 0) {
    if (tens_val == 1 && num != 10) {
      strcat(words, TEENS[ones_val]);
      return strlen(TEENS[ones_val]);
    }
    strcat(words, TENS[tens_val]);
    len += strlen(TENS[tens_val]);
    if (ones_val > 0) {
      strcat(words, " ");
      len += 1;
    }
  }

  if (ones_val > 0 || num == 0) {
    strcat(words, ONES[ones_val]);
    len += strlen(ONES[ones_val]);
  }
  return len;
}

void fuzzy_time_to_words(int hours, int minutes, char* words) {
  int fuzzy_hours = hours;
  int fuzzy_minutes = ((minutes + 2) / 5) * 5;

  // Handle hour & minute roll-over.
  if (fuzzy_minutes > 55) {
    fuzzy_minutes = 0;
    fuzzy_hours += 1;
    if (fuzzy_hours > 23) {
      fuzzy_hours = 0;
    }
  }

  time_to_common_words(fuzzy_hours, fuzzy_minutes, words);
}

// returns number of lines written
void time_to_common_words(int hours, int minutes, char *words) {
  // TODO: make an app-safe assert
  // PBL_ASSERT(hours >= 0 && hours < 24, "Invalid number of hours");
  // PBL_ASSERT(minutes >= 0 && minutes < 60, "Invalid number of minutes");

  size_t written = 0;
  strcpy(words, "");

  if (minutes != 0 && (minutes >= 10 || minutes == 5 || hours == 0 || hours == 12)) {
    if (minutes == 15) {
      written += sprintf(words, "%s %s ", STR_QUARTER, STR_AFTER);
    } else if (minutes == 45) {
      written += sprintf(words, "%s %s ", STR_QUARTER, STR_TO);
      hours = (hours + 1) % 24;
    } else if (minutes == 30) {
      written += sprintf(words, "%s %s ", STR_HALF, STR_PAST);
    } else if (minutes < 30) {
      written += append_number(words, minutes);
      written += sprintf(words + written, " %s ", STR_AFTER);
    } else {
      written += append_number(words, 60 - minutes);
      written += sprintf(words + written, " %s ", STR_TO);
      hours = (hours + 1) % 24;
    }
  }

  if (hours == 0) {
    written += sprintf(words + written, "%s", STR_MIDNIGHT);
  } else if (hours == 12) {
    strcat(words, STR_NOON);
    written += sprintf(words + written, "%s", STR_NOON);
  } else {
    written += append_number(words, hours % 12);
  }

  if (minutes < 10 && minutes != 5 && !(hours == 0 || hours == 12)) {
    if (minutes == 0) {
      sprintf(words + written, " %s%s", STR_OH_TICK, STR_CLOCK);
    } else {
      sprintf(words + written, " %s%s", STR_OH_TICK, ONES[minutes]);
    }
  }
}



// o'clock (0) and plain number words (10..)
void minute_to_formal_words(int minutes, char *first_word, char *second_word) {
  // PBL_ASSERT(minutes >= 0 && minutes < 60, "Invalid number of minutes");

  strcpy(first_word, "");
  strcpy(second_word, "");


  if (minutes == 0) {
    strcat(first_word, STR_OH_TICK);
    strcat(first_word, STR_CLOCK);
    return;
  }
  if (minutes < 10) {
    strcat(first_word, ONES[minutes%10]);
    return;
  }
  if (minutes > 10 && minutes < 20) {
    strcat(first_word, TEENS_SPLIT[(minutes - 10) % 10][0]);
    strcat(second_word, TEENS_SPLIT[(minutes - 10) % 10][1]);
    return;
  }

  strcat(first_word, TENS[minutes / 10 % 10]);

  int minute_ones = minutes % 10;
  if (minute_ones) {
    strcat(second_word, ONES[minute_ones]);
  }
}

void hour_to_12h_word(int hours, char *word) {
  // PBL_ASSERT(hours >= 0 && hours < 24, "Invalid number of hours");
  hours = hours % 12;

  if (hours == 0) {
    hours = 12;
  }

  strcpy(word, "");

  append_number(word, hours);
}


void hour_to_24h_word(int hours, char *words) {
  // PBL_ASSERT(hours >= 0 && hours < 24, "Invalid number of hours");

  hours = hours % 24;

  strcpy(words, "");

  append_number(words, hours);
}
