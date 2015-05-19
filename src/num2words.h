#pragma once

void time_to_common_words(int hours, int minutes, char *words);
void fuzzy_time_to_words(int hours, int minutes, char* words);
void minute_to_formal_words(int minutes, char *first_word, char *second_word);
void hour_to_12h_word(int hours, char *word);
void hour_to_24h_word(int hours, char *words);
