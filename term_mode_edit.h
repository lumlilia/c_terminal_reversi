/* Last update 2024/06/12 */

#pragma once


#define SET_TERM_DEFAULT 0
#define SET_TERM_NONCANON 1

int GetDefaultTerms();
void ChangeTermMode(int);
