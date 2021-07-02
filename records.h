#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define MAX_RECORDS_AMOUNT              10
#define MAX_NAME_FOR_RECORD_LENGTH		10
#define FILENAME_WITH_RECORDS           L"records.bin"

typedef struct Record {
	WCHAR name[MAX_NAME_FOR_RECORD_LENGTH + 1];
	UINT score;
} Record;

typedef struct RecordList {
	Record records[MAX_RECORDS_AMOUNT];
	UINT amount;
} RecordList;

VOID loadRecordList(VOID);
VOID saveRecordList(VOID);
VOID addNewRecord(Record *newRecord);
VOID updateRecordList(VOID);
