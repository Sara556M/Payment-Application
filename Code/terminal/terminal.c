#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../card/Card.h"
#include "terminal/terminal.h"
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	printf("enter the transaction date as following dd/mm/yyyy\n");
	gets(termData->transactionDate);
	for (int i = 0;i < termData->transactionDate;i++)
	{
		if (termData->transactionDate[2] != '/' || termData->transactionDate[5] != '/')
			return WRONG_DATE;
		/*if (i != 2 || i != 5)
		{``
			if(isdigit(termData->transactionDate[i]) != 0)
				return WRONG_DATE;
		}*/
			
	}
	if (strlen(termData->transactionDate) != 10||termData->transactionDate==NULL)
		return WRONG_DATE;
	else
	return OK1;
}
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)

{
	uint8_t TerminalYear[2], TerminalMonth[2], CardYear[2], CardMonth[2];
	TerminalYear[0] = termData.transactionDate[8];
	TerminalYear[1] = termData.transactionDate[9];
	CardYear[0] = cardData.cardExpirationDate[3];
	CardYear[1] = cardData.cardExpirationDate[4];
	TerminalMonth[0] = termData.transactionDate[3];
	TerminalMonth[1] = termData.transactionDate[4];
	CardMonth[0] = cardData.cardExpirationDate[0];
	CardMonth[1] = cardData.cardExpirationDate[1];
	if (atoi(TerminalYear) > atoi(CardYear))
		return EXPIRED_CARD;
	else if (atoi(TerminalYear) == atoi(CardYear)&& atoi(TerminalMonth) > atoi(CardMonth))
	{
		return EXPIRED_CARD;
	}

	return OK;
}
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	printf("enter the transaction amount\n");
	scanf_s("%f", &termData->transAmount);
	if (termData->transAmount <= 0)
		return INVALID_AMOUNT;
	else
		return OK1;
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->maxTransAmount < termData->transAmount)
		return EXCEED_MAX_AMOUNT;
	else
		return OK1;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	termData->maxTransAmount = 2000.00;
	if (termData->maxTransAmount <= 0)
		return INVALID_MAX_AMOUNT;
	else
		return OK1;
}