/*
Example to ilustrate the heap

~~ r2con 2018 ~~

by morbtih-dqtz
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STDINBUF 0x80
#define MAXPTRS 100

/* returns the status of the pointers array */
void stat_ptrs (char *ptrs[MAXPTRS]) {
	int i;
	for (i = 1; i <= MAXPTRS - 1; i++ ) {
		//printf ("idx:%d - (%p) - ", i , *(ptrs + i));
		if ( i % 3 != 0 ) {
			printf ("idx:%.3d - (%p) - ", i , *(ptrs + i));
		} else {
			printf ("idx:%.3d - (%p)\n", i , *(ptrs + i));
		}

	}
}

/* check that idx in the ptrs array is not NULL */
int check_idx (char *ptrs[MAXPTRS], int idx) {
	if (ptrs[idx] != NULL) {
		return 1;
	}
	return 0;
}

/* returns the first free idx in the ptrs array or -1 if is not one*/
int first_free_idx (char *ptrs[MAXPTRS]){
	int i;
	for(i = 1; i <= MAXPTRS - 1; i++) {
		if ( *( ptrs + i ) == NULL ) {
			return i;
		}
	}
	return -1;
}

void main(void) {
	int idx, spc;
	char tmp[STDINBUF] = "", choice[STDINBUF] = "", attr[STDINBUF] = "";
	char *ptrs[MAXPTRS] = { NULL };
	char *p;

	printf ("*.... Heap playground ....*\n");
	while( (strncmp((char *)&choice,"8",1) != 0)) {
		printf ("\n1.Alloc chunk\n");
		printf ("2.Free chunk\n");
		printf ("3.Write into chunk\n");
		printf ("4.Print chunk\n");
		printf ("5.Print ptrs array\n");
		printf ("6.Write into tmp\n");
		printf ("7.Print tmp\n");
		printf ("8.Exit\n>");

		fgets(choice,STDINBUF,stdin);
		/* 1 . Alloc chunk */
		if (strncmp (choice, "1", 1) == 0) {
			idx = first_free_idx (ptrs);
			if (idx != -1) {
				printf ("How many space ?: ");
				fgets(attr, STDINBUF, stdin);
				spc = atoi ((char *)attr);
				if (spc) {
						p = malloc (spc);
						ptrs[idx] = p;
						printf ("idx:%d - (%p)\n", idx, p);
					} else {
						printf ("Error, can't use this value\n");
					}
			} else {
				printf ("There is no free idx to store the pointer\n");
			}
		fflush( stdin );
		}

		/* 2. Free chunk */
		else if (strncmp ((char *)&choice,"2", 1) == 0) {
			printf ("Index to free: ");
			fgets(attr,STDINBUF,stdin);
			idx = atoi(attr);
			if ((idx > 0) && (idx / MAXPTRS < 1)) {
				if (check_idx(ptrs, idx)) {
					free (ptrs[idx]);
					ptrs[idx] = NULL;
				} else {
					printf ("Wrong index\n");
				}
			} else {
				printf ("Wrong index\n");
			}
			fflush( stdin );
		}

		/* 3. Write into chunk */
		else if (strncmp (choice, "3", 1) == 0) {
			printf ("Write index: ");
			fgets(attr,STDINBUF,stdin);
			idx = atoi (attr);
			if ((idx > 0) && (idx / MAXPTRS < 1)) {
					if (check_idx (ptrs, idx )) {
						printf ("Write data: ");
						fgets(attr,STDINBUF,stdin);
						memcpy(* (ptrs + idx), attr, strlen(attr));
				} else {
					printf ("%d\n",idx);
					printf ("Error, we can't use this value\n");
				}
			} else {
					printf ("Wrong index\n");
				}
			fflush( stdin );
		}

		/* 4. Print chunk */
		else if (strncmp (choice, "4", 1) == 0) {
			printf ("Print index :");
			fgets(attr,STDINBUF,stdin);
			idx = atoi (attr);
			if ((idx > 0) && (idx / MAXPTRS < 1)) {

				if (check_idx (ptrs, idx)){
					printf("%d\n", idx);
					printf (*( ptrs + idx), strlen(*( ptrs + idx)));
				} else {
					printf("Wrong index\n");
				}
			} else {
					printf("Wrong index\n");
			}
			fflush( stdin );
		}

		/* 5. Print pointers array */
		else if (strncmp ((char *)&choice,"5", 1) == 0) {
			stat_ptrs (ptrs);
			fflush( stdin );
		}

		/* 6 . Write into tmp array */
		else if (strncmp (choice,"6", 1) == 0) {
			fgets (tmp, STDINBUF, stdin);
			fflush( stdin );
		}

		/* 7. Ptiny tmp array */
		else if (strncmp (choice,"7", 1) == 0) {
			printf (tmp);
			fflush( stdin );
		}

		/* 8. Exit playground */
		else if (strncmp (choice,"8", 1) == 0) {
			printf ("See you soon!\n");
			fflush( stdin );
		}

		else printf("Invalid choice\n");
	}
}
