#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"../card/Card.h"
#include"../terminal/terminal.h"
#include"server/server.h"
ST_accountsDB_t Accountdatabase[255] = { {25,RUNNING,"8945623985431754"},{300,BLOCKED,"8945623985431755"},{70000,RUNNING,"8945623985431756"},{100000,BLOCKED,"8945623985431757"} };
ST_transaction Transatabase[255] = { 0 };
float CardBalance = 0.00;
int CardIndex = 0;
int transactionIndex = 0;
EN_transState_t receiveTransactionData(ST_transaction* transData) {
    int error1 = isValidAccount(&transData->cardHolderData);
    if (error1 == DECLINED_STOLEN_CARD)
    {
        transData->transState = DECLINED_STOLEN_CARD;
        return DECLINED_STOLEN_CARD;

    }
    int error2 = isAmountAvailable(&transData->terminalData);
    if (error2 == LOW_BALANCE)
    {
        transData->transState = LOW_BALANCE;
  
        return DECLINED_INSUFFECIENT_FUND;
    }
    int error3 = saveTransaction(&transData);
    if (error3 == SAVING_FAILED)
    {
        transData->transState = INTERNAL_SERVER_ERROR;
        return INTERNAL_SERVER_ERROR;
    }
    Accountdatabase[CardIndex].balance -= transData->terminalData.transAmount;
    printf("your balance after transaction is %.2f\n", Accountdatabase[CardIndex].balance);
    CardBalance = 0.00;
    CardIndex = 0;
    transData->transState = APPROVED;
    return APPROVED;
}
EN_serverError_t isValidAccount(ST_cardData_t* cardData) {
    for (int i = 0; i<255; i++) {
        if (strcmp(cardData->primaryAccountNumber, Accountdatabase[i].primaryAccountNumber) == 0)
        {
            if (Accountdatabase[i].state == BLOCKED)
            {
                return DECLINED_STOLEN_CARD;
            }
            CardBalance = Accountdatabase[i].balance;
            CardIndex = i;
            return OK2;
        }
    }
    return DECLINED_STOLEN_CARD;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData) {
    if (termData->transAmount > CardBalance)
        return LOW_BALANCE;
    else
        return OK2;
}
EN_serverError_t saveTransaction(ST_transaction* transData) {

    transData->transactionSequenceNumber = transactionIndex;
    if (transData->transactionSequenceNumber < 255)
    {
        Transatabase[transData->transactionSequenceNumber].cardHolderData = transData->cardHolderData;
        Transatabase[transData->transactionSequenceNumber].terminalData = transData->terminalData;
        Transatabase[transData->transactionSequenceNumber].transState = transData->transState;
        Transatabase[transData->transactionSequenceNumber].transactionSequenceNumber = transData->transactionSequenceNumber + 1;
        transactionIndex++;
        return OK2;
    }
    return SAVING_FAILED;
}

