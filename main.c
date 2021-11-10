#include <stdio.h>
#include "getSource.h"

main()
{
   char fileName[30] = "example.txt";
   printf("enter source file name\n");
   //scanf("%s", fileName);
   if (!openSource(fileName))
      return;
   if (compile())
      execute();
	listCode();
    closeSource();
}
