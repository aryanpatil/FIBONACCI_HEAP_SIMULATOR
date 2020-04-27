#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct used_tag
{
    char name[10];
    int start;
    int size;
    struct used_tag *next;
    struct used_tag *prev;
}   used;

typedef struct node_tag
{
    int size;
    int start;
    int occupied;
    struct node_tag *next;
    struct node_tag *prev;
}   node;

// GENERATION OF HEAP MEMORY ( 1 TIME PROCESS )
node *Heap_generator(int x)
{
    node *ptr,*nptr,*last;
    int a=1,b=1,i=1,sum=2;
    //CREATION OF FIRST NODE
    nptr=(node*)malloc(sizeof(node));
    nptr->next=NULL;
    nptr->prev=NULL;
    nptr->start=0;
    nptr->size=1;
    nptr->occupied=0;
    // PTR POINTS TO START OF LIST
    // LAST POINTS TO LAST NODE OF LIST
    ptr=nptr;
    last=nptr;
    //CREATION OF REMAINING NODES
    while(sum<=x)
    {
        nptr=(node*)malloc(sizeof(node));
        nptr->size=i;
        nptr->start=last->size + i - 1;
        nptr->occupied=0;
        nptr->prev=last;
        nptr->next=NULL;
        last->next=nptr;
        last=last->next;
        //INCREMENT OF FIBONACCI NUMBER
        i=a+b;
        a=b;
        b=i;
        sum+=i;
    }

    // RETURNING LAST NODE POINTER
    return ptr;
}

// ALLOCATING MEMORY
used *Allocate(int size,char word[],used *freelist,node **first)
{
    used *ptr,*nptr;
    ptr=freelist;

    node *mem,*begin,*end;
    end=mem=*first;

    int width,satisfied=0,addr,sum=0;
    width=size;

    // SEARCHING FOR CEILING OF MEMORY TO ALLOCATE
    while((mem!=NULL)&&(mem->size<width))
    {
        mem=mem->next;
    }
    // IF MEMORY TO BE ALLOCATED IS LESS THAN GREATEST FIBONACCI NUMBER
    if(mem!=NULL)
    {
        // IF CEILING IS AVAILABLE , DIRECT ALLOTMENT
        if(mem->occupied==0)
        {
            mem->occupied=1;
            satisfied=1;
            sum=mem->size;
            addr=mem->start;
        }
        // ELSE 
        else
        {
            end=mem->prev;
            // SEARCH FOR THE NEXT AVAILABLE MEMORY
            while((mem!=NULL)&&(mem->occupied==1))
            {
                mem=mem->next;
            }
            // IF AVAILABLE THEN ALLOT DIRECTLY
            if(mem!=NULL)
            {
                mem->occupied=1;
                satisfied=1;
                sum=mem->size;
                addr=mem->start;
            }
            // IF NOT AVAILABLE THEN COMBINE PREVIOUS CONTAGIOUS MEMORIES TO ALLOT 
            else
            {
                begin=end;
                while((begin!=NULL)&&(satisfied==0))
                {
                    sum=0;
                    // FINDING CONTAGIOUS MEMORY
                    while((begin!=NULL)&&(sum<width)&&(begin->occupied==0))
                    {
                        sum+=begin->size;
                        begin=begin->prev;    
                    }
                    // IF CONTAGIOUS MEMORY FOUND
                    if(sum>=width)
                    {
                        
                        while(end!=begin)
                        {
                            end->occupied=1;
                            end=end->prev;
                        }
                        satisfied=1;
                        if(begin==NULL)
                        {
                            addr=(*first)->start;
                        }
                        else
                        {
                            addr=begin->next->start;
                        }
                    }
                    // IF NOT FOUND THEN SEARCH FOR NEXT CONTAGIOUS MEMORY AVAILABLE
                    else if((begin!=NULL)&&(begin->occupied==1))
                    {
                        end=begin;
                        while((end!=NULL)&&(end->occupied==1))
                        {
                            end=end->prev;
                        }
                        begin=end;
                    }

                }
            }
        }
    }
    // IF ALLOCATED MEMORY IS GREATER THAN THE LARGEST FIBONACCI NUMBER
    else
    {
        while(end->next!=NULL)
        {
            end=end->next;
        }
        begin=end;
        while((begin!=NULL)&&(satisfied==0))
        {
            sum=0;
            // FINDING CONTAGIOUS MEMORY
            while((begin!=NULL)&&(sum<width)&&(begin->occupied==0))
            {
                sum+=begin->size;
                begin=begin->prev;    
            }
            // IF CONTAGIOUS MEMORY FOUND
            if(sum>=width)
            {
                while(end!=begin)
                {
                    end->occupied=1;
                    end=end->prev;
                }
                satisfied=1;
                if(begin==NULL)
                {
                    addr=(*first)->start;
                }
                else
                {
                    addr=begin->next->start;
                }
            }
            // IF NOT FOUND THEN SEARCH FOR NEXT CONTAGIOUS MEMORY AVAILABLE
            else if((begin!=NULL)&&(begin->occupied==1))
            {
                end=begin;
                while((end!=NULL)&&(end->occupied==1))
                {
                    end=end->prev;
                }
                begin=end;
            }
        }    
    }
    // IF ALLOCATION OF MEMORY IS SUCCESSFUL
    if(satisfied==1)
    {
        printf("Memory allocation successful\n");
        // ADD NODE IN THE ALLOCATED LIST
        nptr=(used*)malloc(sizeof(used));
        strcpy(nptr->name,word);
        nptr->prev=NULL;
        nptr->next=NULL;
        nptr->size=sum;
        nptr->start=addr;
        if(ptr!=NULL)
        {
           ptr->prev=nptr;
           nptr->next=ptr;
        }
        ptr=nptr;
    }
    // IF ALLOCATION IS UNSUCCESSFUL
    else
    {
        printf("Not enough memory available\n");
    }
    return ptr;
}

// RELEASING MEMORY
used *free_mem(used *freelist,node **first,char word[])
{
    used *begin,*ptr,*nptr;
    begin=ptr=freelist;

    node *mem;
    mem=*first;

    int addr,width;

    while((ptr!=NULL)&&(strcmp(word,ptr->name)!=0))
    {
        ptr=ptr->next;
    }
    if(ptr==NULL)
    {
        printf("No memory allocated by the provided name\n");
    }
    else
    {
        addr=ptr->start;
        width=ptr->size;
        while(mem->start!=addr)
        {
            mem=mem->next;
        }
        while(width>0)
        {
            mem->occupied=0;
            width=width-(mem->size);
            mem=mem->next;
        }

        if(ptr->prev==NULL)
        {
            nptr=ptr;
            ptr=ptr->next;
            if(ptr!=NULL)
            {
                ptr->prev=NULL;
            }
            free(nptr);
            begin=ptr;
        }
        else
        {
            nptr=ptr;
            ptr=ptr->prev;
            ptr->next=nptr->next;
            if(nptr->next!=NULL)
            {
                nptr->next->prev=ptr;
            }
            free(nptr);
        }

        printf("The memory has been released\n");
    }
    return begin;
}

// DISPLAY FREE LIST
void Display_free_list(node *first)
{
    node *ptr;
    ptr=first;
    int sum=0,addr;
    printf("FREE MEMORY:\n");
    printf("ADDRESS\tSIZE\n");

    while(ptr!=NULL)
    {
        sum=0;
        while((ptr!=NULL)&&(ptr->occupied==1))
        {
            ptr=ptr->next;
        }
        if(ptr!=NULL)
        {
            addr=ptr->start;
            sum=ptr->size;
            ptr=ptr->next;
            while((ptr!=NULL)&&(ptr->occupied==0))
            {
                sum+=ptr->size;
                ptr=ptr->next;
            }
            printf("%d\t%d\n",addr,sum);
        }
    }
    printf("______________________________________________________________________\n");
}

// DISPLAY ALLOC LIST
void Display_alloc_list(used *alloc_list)
{
    used *ptr;
    ptr=alloc_list;
    printf("UTILIZED MEMORY:\n");
    printf("NAME\tADDRESS\tSIZE\n");
    while(ptr!=NULL)
    {
        printf("%s\t%d\t%d\n",ptr->name,ptr->start,ptr->size);
        ptr=ptr->next;
    }
    printf("______________________________________________________________________\n");

}

void main()
{
    int size;
    // CREATING MEMORY
    printf("ENTER THE SIZE OF MEMORY:\n");
    scanf("%d",&size);
    node *first;
    first=Heap_generator(size);

    // INITIALISING ALLOCATED LIST
    used *alloc_list;
    alloc_list=NULL;

    int exit=0,response=0;
    char word[10];

    // CREATING USER INTERFACE
    while(exit==0)
    {
        // ASKING USER FOR OPERATION
        printf("Press 1 to allocate memory\nPress 2 to free memory\nPress 3 to view free list and allocated list\nPress 4 to exit\n");
        scanf("%d",&response);

        switch(response)
        {
            case 1: printf("Enter the size of memory to be allocated:\n");
                    scanf("%d",&size);
                    printf("Enter the name to allocated memory:\n");
                    scanf("%s",word);
                    alloc_list=Allocate(size,word,alloc_list,&first);
                    break;

            case 2: Display_alloc_list(alloc_list);
                    printf("Enter the name of memory to free it:\n");
                    scanf("%s",word);
                    alloc_list=free_mem(alloc_list,&first,word);
                    break;

            case 3: Display_alloc_list(alloc_list);
                    Display_free_list(first);
                    break;

            case 4: exit=1;
                    break;

            default: printf("Please enter a valid input\n");
                     break;
        }
    }
}