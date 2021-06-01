#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define LEN 1000

double *x;
//double sum;
double sv;
int len;

void* array_sum();

int main (int argc, char* argv[]) {

    omp_set_dynamic(0);
    omp_set_num_threads(2); 
    int tNum = 2;

    len = LEN;
    int i;

    //sum = 0;
    sv = 0;

    x = (double*) malloc (tNum * len * sizeof(double));

    #pragma omp parallel for schedule(static) shared(x) if(len*tNum>1000)
	for (i = 0; i < len * tNum; i++){
		if(i%2 == 0) {
            x[i] = 2;
        } else {
            x[i] = 1;
        }
	}

    #pragma omp parallel shared(x,len) reduction(+:sv)
	{
	    array_sum();
	}

    printf("Done. SV  =  %f \n", sv);

    free(x);

    return 0;
}

void* array_sum() {
    int i, start, end, myId;
    myId = omp_get_thread_num();
    start = myId * len;
    end  = start + len;
    double sum = 0;

    for(i=start; i<end; i++) {
        sum += x[i];
    }

    sv += sum / (2*len);
}