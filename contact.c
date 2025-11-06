#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include<ctype.h>
// Text color macros
#define RESET   "\033[0m"
#define BLACK   "\033[0;30m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"
int is_valid_phone(char *phone);
int is_valid_name(char *name);
int is_valid_email(char *email);
int is_dup_phone(char *phone,AddressBook *addressBook);
int is_dup_email(char *email,AddressBook *addressBook);

void listContacts(AddressBook *addressBook,int sortCriteria) 
{
    
    if(addressBook->Contactcount==0)
    {
        printf(RED"There are no contacts, please add some contacts. \n"RESET);
        return;
    }
    for (int i = 0; i <  addressBook->Contactcount - 1; i++)
    {
        for (int j = 0; j <  addressBook->Contactcount-i-1; j++) 
        {
            int cmp = 0;

            switch (sortCriteria) 
            {
                case 1: // Sort by Name
                    cmp = strcasecmp(addressBook->contacts[j].name, addressBook->contacts[j+1].name);
                    break;
                case 2: // Sort by Email
                    cmp = strcasecmp(addressBook->contacts[j].email, addressBook->contacts[j+1].email);
                    break;
                case 3: // Sort by Phone
                    cmp = strcmp(addressBook->contacts[j].phone,addressBook->contacts[j+1].phone); 
                    break;
                case 4: // Normal order 
                    cmp = 0;
                    break;
            }

            // Swap if out of order (except for normal method)
            if (sortCriteria != 4 && cmp > 0)
            {
                Contact temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j+1];
                addressBook->contacts[j+1] = temp;
            }

            
        }
    }
    printf("-------------------------------------------------------------------------\n");
    printf(MAGENTA"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
    printf("-------------------------------------------------------------------------\n");
    for (int k = 0; k <  addressBook->Contactcount; k++) 
    {
        printf(BLUE"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                    k+1,
                    addressBook->contacts[k].name,
                    addressBook->contacts[k].phone,
                    addressBook->contacts[k].email);
            printf("-------------------------------------------------------------------------\n");

    }

}

void initialize(AddressBook *addressBook) {
    addressBook->Contactcount = 0;
    loadContactsFromFile(addressBook);
    
    // Load contacts from file during initialization (After files)
    //loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
    if(addressBook->Contactcount>=MAX_CONTACTS)
    {
        printf(YELLOW"Addressbook is full.Cannot add more contacts\n"RESET);
        return;
    }
    char name[50];
    char phone[10];
    char email[50];
    //step1 is to enter the name,validate the name if ok then store it and break the loop.
    while(1)
    {
        printf("Enter the name : ");
        scanf(" %[^\n]",name);

        int valid=is_valid_name(name);
        if(valid)
        {
            strcpy(addressBook->contacts[addressBook->Contactcount].name,name);
            break;
        }
        else
        {
            printf(RED"Error , please re-enter the name.\n"RESET);

        }
    }
    
    //step2 is to enter the phone number,validate the phone number if ok then store it and breaak the loop.
    while(1)
    {
        printf("Enter the phone_number : ");
        scanf(" %[^\n]",phone);
        getchar();

        int valid_phone=is_valid_phone(phone);
        if(!valid_phone)
        {
            printf(RED"Error , Please re-enter the phone number.\n"RESET);
            continue;
        }
        int dup_phone=is_dup_phone(phone,addressBook);
        if(!dup_phone)
        {
            printf(RED"Phone number already exists, Please re-enter the phone number.\n"RESET);
            continue;
        }
        if(valid_phone && dup_phone)
        {
            strcpy(addressBook->contacts[addressBook->Contactcount].phone,phone);
            break;
        }
        else
        {
            printf(RED"\n\nPhone number already exists.\n"RESET);
        }
    }

    //step3 is to enter the email-id,validate the email-id if ok then store it and break the loop.
    while(1)
    {
        printf("Enter the email-id : ");
        scanf(" %[^\n]",email);

        int valid_email=is_valid_email(email);
        if(!valid_email)
        {
            printf(RED"Error , Please re-enter the email-id.\n"RESET);
            continue;
        }
        int dup_email=is_dup_email(email,addressBook);
        if(!dup_email)
        {
            printf(RED"Email id alreaady exists , Please re-enter the email-id.\n"RESET);
            continue;
        }
        if(valid_email && dup_email)
        {
            strcpy(addressBook->contacts[addressBook->Contactcount].email,email);
            break;
        }
        else
        {
            printf(RED"\n\nEmail-id already exists\n"RESET);
        }
    }

    (addressBook->Contactcount)++;      //update the contactcount
    printf(GREEN"Contact added Successfully\n"RESET);


}

void searchContact(AddressBook *addressBook) 
{
    char search[50];
    int choice;
    do
    {
        printf("Enter the search menu : \n");
        printf("1 . Name\n2 . Phone number\n3 . Email-id\n4 . Exit\n");
        printf("Enter the choice : ");
        scanf("%d",&choice);
        int found_contact=0;
        int matched_index_name[addressBook->Contactcount];

        switch (choice)
        {
            //step1 is to enter the name which user wants to search , if it is present then display it
            case 1:
            {
                printf("Enter the name to search : ");
                scanf(" %[^\n]",search);
                if(!(is_valid_name(search)))
                {
                    printf(RED"Invalid name,please re-enter name\n"RESET);
                    break;
                }

                found_contact=0;
                for(int i=0;i<addressBook->Contactcount;i++)
                {
                    if(strcasecmp(addressBook->contacts[i].name,search)==0)
                    {
                        matched_index_name[found_contact++]=i;
                    }
                }

                if(found_contact==0)
                {
                    printf(RED"Name not available,please re-enter the name\n"RESET);
                }
                else if(found_contact==1)
                {
                    printf(GREEN"Contact found, details are listed below : \n"RESET);
                    int i = matched_index_name[0];
                    int num=0;
                    printf(MAGENTA"-------------------------------------------------------------------------\n"RESET);
                    printf(BLUE"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                    printf(MAGENTA"-------------------------------------------------------------------------\n"RESET);
                    printf(CYAN"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                    num+1,
                    addressBook->contacts[i].name,
                    addressBook->contacts[i].phone,
                    addressBook->contacts[i].email);  
                    printf(CYAN"-------------------------------------------------------------------------\n"RESET);
                }
                else
                {
                    printf(RED"\nThere are multiple matches found for your name , those are listed below .\n"RESET);
                    printf(MAGENTA"-------------------------------------------------------------------------\n"RESET);
                    printf(BLUE"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                    printf(MAGENTA"-------------------------------------------------------------------------\n"RESET);
                    for(int i=0;i<found_contact;i++)
                    {
                        int index=matched_index_name[i];
                        
                        printf(CYAN"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                        i+1,
                        addressBook->contacts[index].name,
                        addressBook->contacts[index].phone,
                        addressBook->contacts[index].email);  
                        printf(RED"-------------------------------------------------------------------------\n"RESET);
                        

                    }
                    printf("\nPlease enter additional details to identify the contact.\n");
                    
                    char ph[20];
                    printf("\nEnter phone number from above table that you want to search: ");
                    scanf(" %[^\n]",ph);
                    
                    
                    int exact_found = 0,index=0;
                    int matched_index_name_phone[addressBook->Contactcount];
                    for(int i=0;i<found_contact;i++)
                    {
                        int idx = matched_index_name[i];
                        if(strcmp(addressBook->contacts[idx].phone,ph)==0)
                        {
                            exact_found=1;
                            matched_index_name_phone[index++]=idx;
                        }
                    }
                    if(exact_found==0)
                    {
                        printf("No exact contact found with given phone and email.\n");
                    }
                    else if(exact_found==1)
                    {
                        printf(GREEN"\nContact found, details are listed below : \n"RESET);
                        int id=0;
                        int k=matched_index_name_phone[0];
                        printf(CYAN"-------------------------------------------------------------------------\n"RESET);
                        printf(MAGENTA"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                        printf(CYAN"-------------------------------------------------------------------------\n"RESET);
                        printf(BLUE"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                        id+1,
                        addressBook->contacts[k].name,
                        addressBook->contacts[k].phone,
                        addressBook->contacts[k].email);  
                        printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                        
                    }     
                }
            }
            
            break;
        
            //step2 is to enter the phone number which user wants to search if it is present then display it
            case 2:
            {
                printf("Enter the phone number to search : \n");
                scanf(" %[^\n]",search);
                if(!(is_valid_phone(search)))
                {
                    printf(RED"Invalid phone number,please re-enter number\n"RESET);
                    break;
                }

                found_contact=0;
                for(int i=0;i<addressBook->Contactcount;i++)
                {
                    if(strcmp(addressBook->contacts[i].phone,search)==0)
                    {
                        matched_index_name[found_contact++]=i;
                    }
                }

                if(found_contact==0)
                {
                    printf("Phone number not available,please re-enter the number\n");
                }
                else if(found_contact==1)
                {
                    printf(GREEN"Contact found, details are listed below : \n"RESET);
                    int i = matched_index_name[0],id=0;
                    printf("-------------------------------------------------------------------------\n");
                    printf("|S.No|      Name       |         Phone        |         Email            |\n");
                    printf("-------------------------------------------------------------------------\n");
                    printf("| %-2d | %-15s |      %-15s |      %-20s|\n",
                    id+1,
                    addressBook->contacts[i].name,
                    addressBook->contacts[i].phone,
                    addressBook->contacts[i].email);  
                    printf("-------------------------------------------------------------------------\n");
                }
            }
            
            break;

            //step3 is to enter the email-id you want to search
            case 3:
            {
                printf("Enter the email-id to search : ");
                scanf(" %[^\n]",search);
                if(!(is_valid_email(search)))
                {
                    printf(RED"Invalid email-id ,please re-enter the email-id\n"RESET);
                    break;
                }

                found_contact=0;
                for(int i=0;i<addressBook->Contactcount;i++)
                {
                    if(strcasecmp(addressBook->contacts[i].email,search)==0)
                    {
                        matched_index_name[found_contact++]=i;
                    }
                }

                if(found_contact==0)
                {
                    printf("Email-id not available,please re-enter the email-id.\n");
                }
                else if(found_contact==1)
                {
                    printf(GREEN"Contact found, details are listed below : \n"RESET);
                    int i = matched_index_name[0],id=0;
                    printf("-------------------------------------------------------------------------\n");
                    printf("|S.No|      Name       |         Phone        |         Email            |\n");
                    printf("-------------------------------------------------------------------------\n");
                    printf("| %-2d | %-15s |      %-15s |      %-20s|\n",
                    id+1,
                    addressBook->contacts[i].name,
                    addressBook->contacts[i].phone,
                    addressBook->contacts[i].email);  
                    printf("-------------------------------------------------------------------------\n");
                    
                }      
            }

            break;
            
            case 4:
            {
                printf(YELLOW"Back to main menu\n"RESET);
                return;
            }

            default:
            {
                printf(YELLOW"Please enter a valid choice (1 to 4).\n"RESET);
                break;
            }
        }
    } while (choice>0 && choice<5);
}


void editContact(AddressBook *addressBook)
{
    char edit[50];
    char exist[50];
    int choice;
    do
    {
        printf("Enter the edit menu : \n");
        printf("1 . Name\n2 . Phone number\n3 . Email-id\n4 . Exit\n");
        printf("Enter the choice : ");
        scanf("%d",&choice);

        int found_contact=0;
        int matched_index[100];
        int index=0;

        switch (choice)
        {
            case 1:
            {
                printf("Enter the existing name : ");
                scanf(" %[^\n]",exist);
                if(!(is_valid_name(exist)))
                {
                    printf(RED"Invalid name,please re-enter the existing name.\n"RESET);
                    break;
                }

                found_contact=0;
                for(int i=0;i<addressBook->Contactcount;i++)
                {
                    if(strcasecmp(addressBook->contacts[i].name,exist)==0)
                    {
                        matched_index[found_contact++]=i;
                    }
                }

                if(found_contact==0)
                {
                    printf(GREEN"Contact not found.\n"RESET);
                    break;
                }
                else if(found_contact==1)
                {
                    index=matched_index[0];
                }
                else
                {
                    printf(CYAN"Multiple contacts found with name : \n"RESET);
                    printf(MAGENTA"-------------------------------------------------------------------------\n"RESET);
                    printf(MAGENTA"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                    printf("-------------------------------------------------------------------------\n");
                    for(int i=0;i<found_contact;i++)
                    {
                        int idx=matched_index[i];
                        
                        printf(BLUE"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                        i+1,
                        addressBook->contacts[idx].name,
                        addressBook->contacts[idx].phone,
                        addressBook->contacts[idx].email);  
                        printf("-------------------------------------------------------------------------\n");
                        
                    }

                    char ph[20];
                    printf("Enter the phone number : ");
                    scanf(" %[^\n]",ph);

                    int exact_found=0;
                    for(int i=0;i<found_contact;i++)
                    {
                        int idx=matched_index[i];
                        if(strcmp(addressBook->contacts[idx].phone,ph)==0)
                        {
                            exact_found=idx;
                            break;
                        }
                    }

                    if(exact_found==0)
                    {
                        printf(RED"No exact contact found with this phone number.\n"RESET);
                        break;
                    }
                    else
                    {
                        index=exact_found;
                    }
                }

                printf("Enter the new name : ");
                scanf(" %[^\n]",edit);
                if(!(is_valid_name(edit)))
                {
                    printf(RED"Invalid name,please re-enter the new name.\n"RESET);
                    break;
                }
                int i=0;
                strcpy(addressBook->contacts[index].name,edit);
                printf(GREEN"\nAfter editing the contact , the details are : \n"RESET);
                printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                printf(CYAN"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                printf(BLUE"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                    i+1,
                    addressBook->contacts[index].name,
                    addressBook->contacts[index].phone,
                    addressBook->contacts[index].email);  
                printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                
                break;
            }

            case 2:
            {
                printf("Enter the existing phone number : ");
                scanf(" %[^\n]",exist);
                if(!(is_valid_phone(exist)))
                {
                    printf(RED"Invalid phone number, please re-enter.\n"RESET);
                    break;
                }

                found_contact=0;
                for(int i=0;i<addressBook->Contactcount;i++)
                {
                    if(strcmp(addressBook->contacts[i].phone,exist)==0)
                    {
                        matched_index[found_contact++]=i;
                    }
                }

                if(found_contact==0)
                {
                    printf(GREEN"Contact not found.\n"RESET);
                    break;
                }
                else if(found_contact==1)
                {
                    index=matched_index[0];
                    int i=0;
                    printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                    printf(CYAN"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                    printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                    printf(BLUE"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                    i+1,
                    addressBook->contacts[index].name,
                    addressBook->contacts[index].phone,
                    addressBook->contacts[index].email);  
                    printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                 
                }

                printf("Enter the new phone number : ");
                scanf(" %[^\n]",edit);
                if(!(is_valid_phone(edit)))
                {
                    printf(RED"Invalid phone number, please re-enter.\n"RESET);
                    break;
                }
                if(!(is_dup_phone(edit,addressBook)))
                {
                    printf(RED"Phone number already exists\n"RESET);
                    break;
                }
                int i=0;
                strcpy(addressBook->contacts[index].phone,edit);
                printf(CYAN"\nAfter editing the contact , the details are : \n"RESET);
                printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                printf(MAGENTA"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                printf(BLUE"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                i+1,
                addressBook->contacts[index].name,
                addressBook->contacts[index].phone,
                addressBook->contacts[index].email);  
                printf(GREEN"-------------------------------------------------------------------------\n"RESET);   
                break;
            }

            case 3:
            {
                printf("Enter the existing email-id : ");
                scanf(" %[^\n]",exist);
                if(!(is_valid_email(exist)))
                {
                    printf(RED"Invalid email-id, please re-enter.\n"RESET);
                    break;
                }
                
                found_contact=0;
                for(int i=0;i<addressBook->Contactcount;i++)
                {
                    if(strcasecmp(addressBook->contacts[i].email,exist)==0)
                    {
                        matched_index[found_contact++]=i;
                    }
                }

                if(found_contact==0)
                {
                    printf(YELLOW"Contact not found.\n"RESET);
                    break;
                }
                else if(found_contact==1)
                {
                    index=matched_index[0];
                    int i=0;
                    printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                    printf(CYAN"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                    printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                    
                    printf(BLUE"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                    i+1,
                    addressBook->contacts[index].name,
                    addressBook->contacts[index].phone,
                    addressBook->contacts[index].email);  
                    printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                    
                }

                printf("Enter the new email-id : ");
                scanf(" %[^\n]",edit);
                if(!(is_valid_email(edit)))
                {
                    printf(RED"Invalid email-id, please re-enter.\n"RESET);
                    break;
                }
                if(!(is_dup_email(edit,addressBook)))
                {
                    printf(RED"Email-id already exists\n"RESET);
                    break;
                }
                int i=0;
                strcpy(addressBook->contacts[index].email,edit);
                printf(CYAN"\nAfter editing the contact , the details are : \n"RESET);
                printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                printf(MAGENTA"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                printf(BLUE"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                i+1,
                addressBook->contacts[index].name,
                addressBook->contacts[index].phone,
                addressBook->contacts[index].email);  
                printf(GREEN"-------------------------------------------------------------------------\n"RESET);
                
            }
            break;
            case 4:
                printf(YELLOW"Back to main menu.\n"RESET);
                return;

            default:
                printf(RED"Please enter a valid choice (1 to 4).\n"RESET);
                break;
        }

    }while(choice>0 && choice<5);
}


void deleteContact(AddressBook *addressBook)
{
    char delete[50];
    int choice;
    do
    {
        printf("Enter the delete menu : \n");
        printf("1 . Name\n2 . Phone number\n3 . Email-id\n4 . Exit\n");
        printf("Enter the choice : ");
        scanf("%d",&choice);

        int contact_found=0;

        switch(choice)
        {
            case 1:
            {
                printf("Enter the name you want to delete : ");
                scanf(" %[^\n]",delete);
                if(!(is_valid_name(delete)))
                {
                    printf(RED"Invalid name, please re-enter the name.\n"RESET);
                    break;
                }

                int matchedIndex[50];
                int matchCount=0;
                for(int i=0;i<addressBook->Contactcount;i++)
                {
                    if(strcasecmp(addressBook->contacts[i].name,delete)==0)
                    {
                        if(matchCount==0)
                        {
                            printf(CYAN"-------------------------------------------------------------------------\n"RESET);
                            printf(MAGENTA"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                            printf(CYAN"-------------------------------------------------------------------------\n"RESET);

                        }
                        printf(BLUE"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                        matchCount+1,
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);  
                        printf(CYAN"-------------------------------------------------------------------------\n"RESET);
                        
                        matchedIndex[matchCount++]=i;
                        contact_found++;
                    }
                }

                if(contact_found==0)
                {
                    printf(RED"No contact found with the given name.\n"RESET);
                    break;
                }
                else if(contact_found==1)
                {
                    int idx=matchedIndex[0];
                    for(int j=idx;j<addressBook->Contactcount-1;j++)
                    {
                        addressBook->contacts[j]=addressBook->contacts[j+1];
                    }
                    (addressBook->Contactcount)--;
                    printf(GREEN"\nContact deleted Successfully.\n"RESET);
                    return ;
                }
                int select;
                printf("\nEnter the Serial number which you want to delete : ");
                scanf("%d",&select);

                int delindex=matchedIndex[select-1];
                for(int j=delindex;j<addressBook->Contactcount-1;j++)
                {
                    addressBook->contacts[j]=addressBook->contacts[j+1];
                }
                (addressBook->Contactcount)--;
                

                printf(GREEN"\n\nContact deleted successfully.\n"RESET);
            }
            
            break;

            case 2:
            {
                printf("Enter the phone number you want to delete : \n");
                scanf(" %[^\n]",delete);
                if(!(is_valid_phone(delete)))
                {
                    printf(RED"Invalid phone number, please re-enter the phone number.\n"RESET);
                    break;
                }

                int matchedIndex[50];
                int matchCount=0;
                for(int i=0;i<addressBook->Contactcount;i++)
                {
                    if(strcmp(addressBook->contacts[i].phone,delete)==0)
                    {
                        if(matchCount==0)
                        {
                            printf(CYAN"-------------------------------------------------------------------------\n"RESET);
                            printf(MAGENTA"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                            printf(CYAN"-------------------------------------------------------------------------\n"RESET);

                        }
                        printf(BLUE"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                        matchCount+1,
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);  
                        printf(CYAN"-------------------------------------------------------------------------\n"RESET);
                        
                        matchedIndex[matchCount++]=i;
                        contact_found++;
                    }
                }

                if(contact_found==0)
                {
                    printf(RED"No contact found with the given phone number.\n"RESET);
                    break;
                }
                else if(contact_found==1)
                {
                    int idx=matchedIndex[0];
                    for(int j=idx;j<addressBook->Contactcount-1;j++)
                    {
                        addressBook->contacts[j]=addressBook->contacts[j+1];
                    }
                    (addressBook->Contactcount)--;
                    printf(GREEN"\nContact deleted Successfully.\n"RESET);
                    return ;
                }
  
            }
            
            break;
            case 3:
            {
                printf("Enter the email-id you want to delete : \n");
                scanf(" %[^\n]",delete);
                if(!(is_valid_email(delete)))
                {
                    printf(RED"Invalid email-id, please re-enter the email-id.\n"RESET);
                    break;
                }

                int matchedIndex[50];
                int matchCount=0;

                for(int i=0;i<addressBook->Contactcount;i++)
                {
                    if(strcasecmp(addressBook->contacts[i].email,delete)==0)
                    {
                        if(matchCount==0)
                        {
                           printf(CYAN"-------------------------------------------------------------------------\n"RESET);
                            printf(MAGENTA"|S.No|      Name       |         Phone        |         Email            |\n"RESET);
                            printf(CYAN"-------------------------------------------------------------------------\n"RESET);


                        }
                        printf(BLUE"| %-2d | %-15s |      %-15s |      %-20s|\n"RESET,
                        matchCount+1,
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);  
                        printf(CYAN"-------------------------------------------------------------------------\n"RESET);
                        

                        matchedIndex[matchCount++]=i;
                        contact_found++;
                    }
                }

                if(contact_found==0)
                {
                    printf(RED"No contact found with the given email-id.\n"RESET);
                    break;
                }
                else if(contact_found==1)
                {
                    int idx=matchedIndex[0];
                    for(int j=idx;j<addressBook->Contactcount-1;j++)
                    {
                        addressBook->contacts[j]=addressBook->contacts[j+1];
                    }
                    (addressBook->Contactcount)--;
                    printf(GREEN"\nContact deleted Successfully.\n"RESET);
                    return ;
                }

            }
            
            break;
            
            case 4:
                printf("Back to main menu.\n");
                return ;

            default:
                printf(RED"Please enter a valid choice (1 to 3).\n"RESET);
                break;
        }
    } while (choice>0 && choice<5);
}


int is_valid_name(char *name)
{
    if(strlen(name)==0)
        return 0;
    if(!(isalpha(name[0])))
        return 0;
    for(int i=1;name[i]!='\0';i++)
    {
        if(!(isalpha(name[i]) || (name[i]==' ')))
            return 0;
    }
    return 1;
}
int is_dup_phone(char *phone,AddressBook *addressBook)
{
    for(int i=0;i<addressBook->Contactcount;i++)
    {
        if(strcmp(addressBook->contacts[i].phone,phone)==0)
        {
            return 0;
        }
    }
    return 1;
}
int is_valid_phone(char *phone)
{
    if(strlen(phone)!=10)
    {
        return 0;
    }
    for(int i=0;phone[i]!='\0';i++)
    {
        if(phone[i]<'0' || phone[i]>'9')
        {
            return 0;
        }
    }

    return 1;
    
}
int is_dup_email(char *email,AddressBook *addressBook)
{
    for(int i=0;i<addressBook->Contactcount;i++)
    {
        if(strcmp(addressBook->contacts[i].email,email)==0)
        {
            return 0;
        }
    }
    return 1;
}
int is_valid_email(char *email)
{
    int at_pos=-1, dot_pos=-1;
    int len = strlen(email);

    if(len < 5 || !isalpha(email[0])) 
    {
        printf(RED"Email should contain atleast 5 characters\n"RESET);
        return 0;   
    }

    for(int i=0; email[i]!='\0'; i++)
    {
        if(email[i]=='@') at_pos=i;
        if(email[i]=='.') dot_pos=i;
    }
    if(strcmp(email+len-4,".com")!=0)
    {
        printf(RED"Error,email should end with '.com'\n "RESET);
        return 0;
    }
    if(at_pos < 1 || dot_pos < at_pos+2 || dot_pos >= len-1)
    {
        printf(RED"Error,Email should contain '@' and '.'\n"RESET);
        return 0;
    }

    return 1;
}


