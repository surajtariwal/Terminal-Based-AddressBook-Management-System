#include <stdio.h>
#include "file.h"
#define RESET   "\033[0m"
#define BLACK   "\033[0;30m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"

void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("contacts.txt", "w");
    if (fptr == NULL)
    {
        printf(RED"Cannot open the file for writing\n"RESET);
        return;
    }

    // Write number of contacts first
    fprintf(fptr, "%d\n", addressBook->Contactcount);

    // Write each contact
    for (int i = 0; i < addressBook->Contactcount; i++)
    {
        fprintf(fptr,"%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fptr);
    printf(GREEN"Contacts saved successfully.\n"RESET);
}


void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("contacts.txt", "r"); 
    if (fptr == NULL)
    {
        printf(RED"No previous contacts found. Starting with an empty address book.\n"RESET);
        addressBook->Contactcount = 0;
        return;
    }

    int num;
    fscanf(fptr,"%d\n",&num);
    for(int i=0 ; i<num && i<MAX_CONTACTS;i++)
    {
        fscanf(fptr,"%[^,],%[^,],%[^\n]\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
        
        addressBook->Contactcount++;
        
    }
    fclose(fptr);
}
