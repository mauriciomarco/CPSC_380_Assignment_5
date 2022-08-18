#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CYLINDERS 5000
#define REQUESTS 1000

int initial_position = 0;
int disk[REQUESTS];

int FCFS(int *input) { /* FCFS algorithm */ 
	int i, head_movement = 0;
	int start = disk[initial_position];
	
	for(i = initial_position; i < REQUESTS; i++) { /* loop through list from head */
		head_movement += abs(disk[i] - start);
	}
	for (i = 0; i < initial_position; i++) { /* loop through remainder of list */
		head_movement += abs(start - input[i]);
	}
	return head_movement;
}

int SSTF(int *input) { /* SSTF algorithm */
	
	int small = initial_position - 1, big = initial_position + 1; /* idx for differences in seek time */
	int small_diff = 0, big_diff = 0;
	int head_movement = 0, num = REQUESTS - 2;
	int head = initial_position, head_val = disk[initial_position];
	
	while(num >= 0) {
		small_diff = abs(disk[head] - disk[small]); /* seek time for idx -1 */
		big_diff = abs(disk[big] - disk[head]); /* seek time for idx + 1 */
		
		if (small_diff < big_diff) { /* add smallest seek time */
			head_movement += small_diff;
			head = small;
			small--;
		}
		else {
			head_movement += big_diff;
			big++;
		}
		num--; /* decrement amount of requests left */
	}
	return head_movement;
}

int SCAN(int * input) { /* scan algorithm */
	int i, curr = 0;
	int saved_val = disk[initial_position], diff = 0;
	int head_movement;
	
	for(i = initial_position - 1; i >= 0; --i) { /* loop from left of head to start of list */
		curr = disk[i];
		diff = abs(saved_val - curr);
		head_movement += diff;
		saved_val = curr;
	}
	
	head_movement+= saved_val;
	saved_val = 0;
	for(i = initial_position + 1; i < REQUESTS; i++) { /* loop from right of intial to end of file */ 
		curr = disk[i];
		diff = abs(curr - saved_val);
		head_movement += diff;
		saved_val = curr;
	}
	return head_movement;
}

int CSCAN(int *input) { /* cscan algorithm */
	int i, curr = 0;
	int saved_val = disk[initial_position], diff = 0;
	int head_movement = 0, last_idx = 4999;
	
	for(i = initial_position + 1; i < REQUESTS; i++) { /* loop from right of head to end of list */
		curr = disk[i];
		diff = abs(saved_val - curr);
		head_movement += diff;
		saved_val = curr;
	}
	head_movement += last_idx - saved_val;
	saved_val = 0;
	head_movement += 4999;
	
	for(i = 0; i < initial_position; i++) { /* loop from start of list to head */ 
		curr = disk[i];
		diff = abs(curr - saved_val);
		head_movement += diff;
		saved_val = curr;
	}
	
	return head_movement;
}

int main(int argc, char *argv[]) {
	int i = 0;
	int cylinder;
	initial_position = atoi(argv[1]);
	
	if (argc != 3) { /* check if correct number of arguments are passed */
		printf("Error: Incorrect number of arguments... {FORMAT: ./diskscheduler <initial position> <cylinder file>\n");
		exit(1);
	}
	if (initial_position > 1000 || initial_position < 0) { /* check for argv[1] */
		perror("Error: Incorrect argument value... please enter an initial position from 0-1000\n");
		exit(1);
	}
	
	FILE *cylinder_file = fopen(argv[2], "r"); /* open file for reading */
	if (cylinder_file == NULL) {
		perror("ERORR: file not found... cylinders.txt\n");
		exit(1);
	}
			
	while(fscanf(cylinder_file, "%d", &cylinder) > 0) { /* read until eof */
		disk[i] = cylinder;
		i++;
	}
	
	printf("[Initial Value at Position %d is %d]\n", initial_position, disk[initial_position]); /* print output */ 
	printf("[FCFS Head Movement %d]\n", FCFS(disk));
	printf("[SSTF Head Movement %d]\n", SSTF(disk));
	printf("[SCAN Head Movement %d]\n", SCAN(disk));
	printf("[CSCAN Head Movement %d]\n", CSCAN(disk));

	fclose(cylinder_file); /* close file */
	
	return 0;
}