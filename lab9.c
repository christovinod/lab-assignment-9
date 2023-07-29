#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

struct Node
{
    struct RecordType data;
    struct Node* next;
};

// Fill out this structure
struct HashType
{
	struct Node** chains;
    int size;
};

// Compute the hash function
int hash(int x, int size)
{
	return x % size;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		if (pHashArray->chains[i] != NULL) // Check if the chain is not empty
        {
            printf("index %d -> ", i);

            struct Node* currentNode = pHashArray->chains[i];
            while (currentNode != NULL)
            {
                printf("%d %c %d", currentNode->data.id, currentNode->data.name, currentNode->data.order);

                if (currentNode->next != NULL)
                {
                    printf(" -> ");
                }

                currentNode = currentNode->next;
            }

            printf("\n");
        }
	}
}

void insertRecord(struct HashType* hashTable, struct RecordType record)
{
    int index = hash(record.id, hashTable->size);
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed while inserting record.\n");
        exit(-1);
    }
    newNode->data = record;
    newNode->next = hashTable->chains[index];
    hashTable->chains[index] = newNode;
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	
	int hashSz = recordSz * 2; // You can choose the hash table size as needed (twice the number of records as a general rule)
    struct HashType hashTable;
    hashTable.chains = (struct Node**)malloc(sizeof(struct Node*) * hashSz);
    hashTable.size = hashSz;

    // Initialize the hash table chains with NULL (no records present at the beginning)
    for (int i = 0; i < hashSz; ++i) {
        hashTable.chains[i] = NULL;
    }

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i) {
        struct RecordType* pRecord = pRecords + i;
        insertRecord(&hashTable, *pRecord);
    }

    // Display records in the hash structure
    displayRecordsInHash(&hashTable, hashSz);

    // Free the memory allocated for the hash table and the parsed records
    for (int i = 0; i < hashSz; ++i) {
        struct Node* currentNode = hashTable.chains[i];
        while (currentNode != NULL) {
            struct Node* nextNode = currentNode->next;
            free(currentNode);
            currentNode = nextNode;
        }
    }
    free(hashTable.chains);
    free(pRecords);

	return 0;
}