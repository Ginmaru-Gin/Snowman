#include "records.h"

#include "variables.h"

int CALLBACK recordsComparator(const void *first, const void *second) {
	return ((Record *)second)->score - ((Record *)first)->score;
}

VOID loadRecordList(VOID) {
	FILE *file;
	if (!_wfopen_s(&file, FILENAME_WITH_RECORDS, L"rb")) {
		fread(&recordList.amount, sizeof(recordList.amount), 1, file);
		fread(recordList.records, sizeof(Record), recordList.amount, file);
		fclose(file);
	}
	else {
		recordList.amount = 0;
	}
}

VOID saveRecordList(VOID) {
	FILE *file;
	if (!_wfopen_s(&file, FILENAME_WITH_RECORDS, L"wb")) {
		fwrite(&recordList.amount, sizeof(recordList.amount), 1, file);
		fwrite(recordList.records, sizeof(Record), recordList.amount, file);
		fclose(file);
	}
}

VOID addNewRecord(Record *newRecord) {
	Record *ptr = recordList.records + min(MAX_RECORDS_AMOUNT - 1, recordList.amount);
	ptr->score = newRecord->score;
	lstrcpyW(ptr->name, newRecord->name);
	recordList.amount = min(MAX_RECORDS_AMOUNT, recordList.amount + 1);

	qsort(recordList.records, recordList.amount, sizeof(Record), recordsComparator);
	saveRecordList();
}

VOID updateRecordList(VOID) {
	for (int i = 0; i < recordList.amount; ++i) {
		LPWSTR strScore;
		LPWSTR record;
		if (strScore = (LPWSTR)malloc(sizeof(WCHAR) * MAX_SCORE_STR_LEN)) {
			_ui64tow_s(recordList.records[i].score, strScore, MAX_SCORE_STR_LEN, 10);
			if (record = (LPWSTR)malloc(sizeof(WCHAR) * ((UINT)lstrlenW(strScore) + (UINT)lstrlenW(recordList.records[i].name) + (UINT)4))) {
				lstrcpyW(record, recordList.records[i].name);
				lstrcpyW(record + lstrlenW(recordList.records[i].name), L"\t:\t");
				lstrcpyW(record + lstrlenW(recordList.records[i].name) + 3, strScore);
				SetWindowTextW(hRecordListLabels[i], record);
			}
		}
	}
}
