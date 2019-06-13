#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
int main()
{
  
    int key,value;
    char b1[100] ,b2[100] ,b3[100] ,b4[100]  ;
    char strline[100];
    
    int i=1;
    while(i){
        fgets(strline,20, stdin);
        if (strlen(strline) != 1)
        {
            sscanf(strline,"(%s %s",b1,b2);
            sscanf(b1,"%[^,]",b3);
            sscanf(b2,"%[^)]",b4);
            ///printf("b3 = ")
            key = atoi(b3);
            printf("key = %d\n",key);
            value = atoi(b4);
            printf("value = %d\n",value);
        }
        else i= 0;
        
        strcpy(b1,"");
        strcpy(b2,"");
        strcpy(b3,"");
        strcpy(b4,"");

    }
  
  /*
   FILE *file = fopen("File1.txt","r");
   int i,j;
   ///fscanf(file,"(%d, %d) ",&i,&j);
   while(fscanf(file,"(%d, %d) ",&i,&j) != EOF)
   {
    printf("i = %d, j = %d",i, j);
    ///fscanf(file,"(%d, %d)",&i,&j);
   }
   fclose(file);  */
}



