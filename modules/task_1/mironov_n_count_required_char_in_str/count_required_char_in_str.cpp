  // Copyright 2022 Mironov Nikita
#include "../../../modules/task_1/mironov_n_count_required_char_in_str/count_required_char_in_str.h"

void getRandomStr(char* str, int strSize) {
    const char arr[] = "abcdefghijklmnopqrstuvwxyz";
    if (strSize <= 0)
        throw "Size can't be negative";
    for (int i = 0; i < strSize; i++) {
        str[i] = arr[rand() % (strlen(arr))];
    }
    str[strSize] = '\0';
}

int countRequiredCharInProcStr(const char* str, char requiredChar) {
    int charCount = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == requiredChar)
            charCount++;
    }
    return charCount;
}

int countRequiredCharInStr(std::string str, char requiredChar) {
    int procCount, procId;

    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    int errorCode = 0;

    if (str.length() == 0) {
        errorCode = 1;
    }
    MPI_Bcast(&errorCode, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (errorCode != 0) {
        throw "errorCode = 1 : Str can't be empty.";
    }

    if (procCount == 1) {
        return countRequiredCharInProcStr(str.c_str(), requiredChar);
    }

    int oneProcWorkAmount = str.length() / procCount;
    int nonDistWork = str.length() % procCount;

    int* oneProcWorkAmountForScatterv;
    int* scattervDispls;

    int procCharCount = 0;
    int resultCount = 0;

    MPI_Comm commForProcInUse;
    MPI_Comm commForNonUsedProc;

    if (oneProcWorkAmount == 0) {
        int color;
        if (procId < nonDistWork) {
            color = 1;
            MPI_Comm_split(MPI_COMM_WORLD, color, procId, &commForProcInUse);
            MPI_Comm_size(commForProcInUse, &procCount);
            MPI_Comm_rank(commForProcInUse, &procId);
            oneProcWorkAmount = 1;
            oneProcWorkAmountForScatterv = new int[str.length()];
            for (int i = 0; i < str.length(); i++) {
                oneProcWorkAmountForScatterv[i] = 1;
            }
        } else {
            color = 2;
            MPI_Comm_split(MPI_COMM_WORLD, color, procId, &commForNonUsedProc);
            MPI_Comm_size(commForNonUsedProc, &procCount);
            MPI_Comm_rank(commForNonUsedProc, &procId);
            MPI_Finalize();
            return 0;
        }
    } else {
        commForProcInUse = MPI_COMM_WORLD;
        oneProcWorkAmountForScatterv = new int[procCount];;
        for (int i = 0; i < procCount; i++) {
            oneProcWorkAmountForScatterv[i] = oneProcWorkAmount;
            if (i+1 == procCount && nonDistWork != 0) {
                oneProcWorkAmountForScatterv[i] =
                    oneProcWorkAmount + nonDistWork;
            }
        }
    }

    if (procId == 0) {
        int messMove = 0;
        for (int proc = 1; proc < procCount; proc++) {
            messMove += oneProcWorkAmount;
            std::string procStr = str.substr(messMove, oneProcWorkAmountForScatterv[proc]);
            MPI_Send(procStr.c_str(),
            oneProcWorkAmountForScatterv[proc], MPI_CHAR, proc, 1,
            commForProcInUse);
        }
        std::string procStr = str.substr(0, oneProcWorkAmountForScatterv[0]);
        procCharCount = countRequiredCharInProcStr(procStr.c_str(), requiredChar);
    }

    if (procId != 0) {
        char* procStr = new char[oneProcWorkAmountForScatterv[procId] + 1];
        MPI_Status status;
        MPI_Recv(procStr, oneProcWorkAmountForScatterv[procId],
            MPI_CHAR, 0, 1, commForProcInUse, &status);
        procStr[oneProcWorkAmountForScatterv[procId]] = '\0';
        procCharCount = countRequiredCharInProcStr(procStr, requiredChar);
    }

    MPI_Reduce(&procCharCount, &resultCount,
        1, MPI_INT, MPI_SUM, 0, commForProcInUse);

    return resultCount;
}
