#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dlElement
{
    int currentIndex;

    struct dlElement *prev;
    struct dlElement *next;

    void *indexData; // име или друго, което може да се ползва за търсене (низ, число и т.н.). Вероятно динамично разпределено.
    void *data;      // указател към същинската информация, която елементът съдържа, може да се разпределя / изтрива динамично.
} dlElement;

typedef struct dlList
{
    char *listName;
    int numElements;

    dlElement *front;
    dlElement *tail;
} dlList;

int addElementAtBeginning(dlList *, dlElement *);
int addElementAtEnd(dlList *, dlElement *);
int addElementAtPosition(dlList *, dlElement *, int);
int deleteElementAtPosition(dlList *, int, void (*)());
int deleteElementAtEnd(dlList *, void (*)());
int deleteElementAtBeginning(dlList *, void (*)());
int dleteElementsBeforeIndex(dlList *, int, void (*)());
int dleteElementsAfterIndex(dlList *, int, void (*)());
int comparator(dlElement *element1, dlElement *element2);
int reorderList(dlList *, int (*)());
dlElement *searchByIndex(dlList *, int, void (*)());
dlElement *searchByIndexData(dlList *, char *, void (*)());

int main()
{

    return EXIT_SUCCESS;
}

int addElementAtPosition(dlList *myList, dlElement *newElement, int index)
{
    if (index < 0 || index > myList->numElements)
    {
        fprintf(stderr, "Index is not valid");
        return -1;
    }
    else
    {
        if (index == myList->numElements)
        {
            addElementAtEnd(myList, newElement);
        }
        dlElement *currentElement = myList->front;
        while (currentElement->currentIndex != index)
        {
            currentElement = currentElement->next;
        }
        newElement->prev = currentElement->prev;
        currentElement->prev = newElement;
        newElement->next = currentElement;
        newElement->currentIndex = currentElement->currentIndex;
        currentElement->currentIndex++;
        myList->numElements++;
        if (!newElement->currentIndex)
        {
            myList->front = currentElement;
        }
        while (currentElement->next)
        {
            currentElement->next->currentIndex++;
            currentElement = currentElement->next;
        }
    }
    return 0;
}

int addElementAtBeginning(dlList *myList, dlElement *newElement)
{
    if (addElementAtPosition(myList, newElement, 0))
    {
        fprintf(stderr, "Operation failed");
        return -1;
    }
    return 0;
}

int addElementAtEnd(dlList *myList, dlElement *newElement)
{
    if (!myList->numElements)
    {
        myList->front = myList->tail = newElement;
        myList->numElements = 1;
        newElement->next = newElement->prev = NULL;
        newElement->currentIndex = 0;
    }
    else
    {
        myList->tail->next = newElement;
        newElement->next = NULL;
        newElement->currentIndex = myList->tail->currentIndex + 1;
        myList->tail = newElement;
    }
    return 0;
}

int deleteElementAtPosition(dlList *myList, int index, void (*func)())
{
    if (index < 0 || index > myList->numElements - 1 || !myList->numElements)
    {
        fprintf(stderr, "Invalid index or emty list");
        return -1;
    }
    dlElement *currentElement = myList->front;
    while (currentElement->currentIndex != index)
    {
        currentElement = currentElement->next;
    }
    if (currentElement->next && currentElement->prev)
    {
        currentElement->prev->next = currentElement->next;
        currentElement->next->prev = currentElement->prev;
        myList->numElements--;
        dlElement *tmpElement = currentElement;
        while (currentElement->next)
        {
            currentElement->next->currentIndex--;
            currentElement = currentElement->next;
        }
        free(tmpElement);
    }
    else if (myList->numElements == 1)
    {
        free(currentElement);
        myList->front = myList->tail = NULL;
    }
    else if (!currentElement->prev)
    {
        currentElement->next->prev = NULL;
        myList->front = currentElement->next;
        myList->numElements--;
        free(currentElement);
    }
    else if (!currentElement->next)
    {
        currentElement->prev->next = NULL;
        myList->tail = currentElement->prev;
        myList->numElements--;
        free(currentElement);
    }
    return 0;
}

int deleteElementAtEnd(dlList *myList, void (*func)())
{
    if (!deleteElementAtPosition(myList, myList->tail->currentIndex, NULL))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int deleteElementAtBeginning(dlList *myList, void (*func)())
{
    if (!deleteElementAtPosition(myList, 0, NULL))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int dleteElementsBeforeIndex(dlList *myList, int index, void (*func)())
{
    if (index >= 0 && index <= myList->tail->currentIndex && myList->numElements)
    {
        dlElement *currentElement = myList->front;
        while (currentElement->currentIndex != index)
        {
            currentElement = currentElement->next;
        }
        if (currentElement->next)
        {
            currentElement->next->prev = NULL;
            myList->front = currentElement->next;
        }
        else
        {
            myList->front = myList->tail = NULL;
        }
        while (currentElement)
        {
            dlElement *tmpElement = currentElement;
            currentElement = currentElement->prev;
            free(tmpElement);
            myList->numElements--;
        }
    }
    else
    {
        fprintf(stderr, "No elements to be deleted or index is invalid");
        return -1;
    }
    return 0;
}

int dleteElementsAfterIndex(dlList *myList, int index, void (*func)())
{
    if (index >= 0 && index <= myList->tail->currentIndex && myList->numElements)
    {
        dlElement *currentElement = myList->front;
        while (currentElement->currentIndex != index)
        {
            currentElement = currentElement->next;
        }
        if (currentElement->prev)
        {
            currentElement->prev->next = NULL;
            myList->tail = currentElement->prev;
        }
        else
        {
            myList->front = myList->tail = NULL;
        }
        while (currentElement)
        {
            dlElement *tmpElement = currentElement;
            currentElement = currentElement->next;
            free(tmpElement);
        }
    }
    else
    {
        fprintf(stderr, "No elements to be deleted or index is invalid");
        return -1;
    }
    return 0;
}

int comparator(dlElement *element1, dlElement *element2)
{
    if (*((int *)element1->data) > *((int *)element2->data))
    {
        return -1;
    }
    else if (*((int *)element1->data) < *((int *)element2->data))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int reorderList(dlList *myList, int (*comparator)(const dlElement *element1, const dlElement *element2))
{
    if (myList->numElements < 2)
    {
        fprintf(stderr, "Nothing to reorder");
        return -1;
    }
    dlElement *currentElement = myList->front;
    int changes = 0;
    while (currentElement->next)
    {
        if (comparator(currentElement, currentElement->next) == -1)
        {
            changes = 1;
            if (currentElement->prev)
            {
                currentElement->prev->next = currentElement->next;
            }
            if (currentElement->next->next)
            {
                currentElement->next->next->prev = currentElement;
            }
            dlElement tmpElement = *currentElement;
            currentElement->prev = currentElement->next;
            currentElement->next = currentElement->next->next;
            tmpElement.next->next = currentElement;
            tmpElement.next->prev = tmpElement.prev;
            currentElement->currentIndex ^= currentElement->prev->currentIndex ^= currentElement->currentIndex ^= currentElement->prev->currentIndex;
        }
    }
    if (changes)
    {
        reorderList(myList, comparator);
    }
    return 0;
}

dlElement *searchByIndex(dlList *myList, int index, void (*func)())
{
    if (myList->numElements)
    {
        dlElement *currentElement = myList->front;
        while (currentElement)
        {
            if (currentElement->currentIndex == index)
            {
                return currentElement;
            }
        }
        return NULL;
    }
}

dlElement *searchByIndexData(dlList *myList, char *indexData, void (*func)())
{
    if (myList->numElements)
    {
        dlElement *currentElement = myList->front;
        while (currentElement)
        {
            if (!strcmp(*((char **)currentElement->indexData), indexData))
            {
                return currentElement;
            }
        }
        return NULL;
    }
}