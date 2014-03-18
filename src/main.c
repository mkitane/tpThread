#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "main.h"
#include <pthread.h>


static pthread_mutex_t lock;


int is_prime(uint64_t p)
{
    if(p==2){
        return 1; 
    }
    //Test par 2
    if(p%2==0){
        return 0;
    }

    uint64_t i;
    //Si echoue on verifie qu'avec les nombres impaires
    for(i=3 ; i<(int)sqrt(p) ; i+=2){
        if(p%i==0){
            return 0;
        }
    }
    return 1; //si premier

}

void find_prime_factors(uint64_t n)
{
    uint64_t i; 

    if(is_prime(n) == 1){
        printf("%llu \n",n);
        return;
    }
    
    for(i=2; i< n ; i++){
        if(n%i == 0 && is_prime(i) == 1){
            printf("%llu ", i);
            break;
        }
    }
	find_prime_factors((uint64_t)n/i);
}


void print_prime_factors(uint64_t n)
{
    printf("%llu : ",n);
    find_prime_factors(n);

    return;
}

void open_file_and_find_prime_factors()
{
    FILE *f = fopen("numbers.txt", "r");
    
    uint64_t number;
    
    while(fscanf(f, "%llu",&number) != EOF) {
        print_prime_factors(number);
    }
	fclose(f);
}

void open_file_and_find_prime_factors_multithread()
{
    //open file
    FILE *f = fopen("numbers.txt", "r");
 
    uint64_t numberOne, numberTwo;
    
    while(fscanf(f, "%llu",&numberOne) != EOF && fscanf(f, "%llu",&numberTwo) != EOF ) { //read file
        pthread_t firstThread , secondThread;
        int crdu;
        
        
        //create first thread
        crdu = pthread_create(&firstThread,NULL,(void*)print_prime_factors,(void*)numberOne);
        if(crdu !=0)
            return;
            
        //create second thread
        crdu = pthread_create(&secondThread,NULL,(void*)print_prime_factors,(void*)numberTwo);
        if(crdu != 0)
            return;
        
        
        //Wait for threads to finish
        crdu = pthread_join(firstThread, NULL);
        crdu = pthread_join(secondThread, NULL);

        //print_prime_factors(number);
    }
    
	fclose(f);
}

void open_file_and_find_prime_factors_workerthread()
{
    //open file
    FILE *f = fopen("numbers.txt", "r");
    
    pthread_t firstThread , secondThread;
    int crdu;
    
    
    pthread_mutex_init(&lock, NULL);     //create mutex in order to use fscanf safely and printf
    
    //create first thread
    crdu = pthread_create(&firstThread,NULL,(void*)readNumber,(void*)f);
    if(crdu !=0)
        return;
    
    //create second thread
    crdu = pthread_create(&secondThread,NULL,(void*)readNumber,(void*)f);
    if(crdu != 0)
        return;
    


    //Wait for threads to finish
    crdu = pthread_join(firstThread, NULL);
    crdu = pthread_join(secondThread, NULL);
    
    
    
    pthread_mutex_destroy(&lock);
    fclose(f);
}

void readNumber(FILE *f)
{
    uint64_t number;
    int bytesRead = 0 ;
    
    
    for(;;){ //read file
        
        pthread_mutex_lock(&lock);
        bytesRead = fscanf(f, "%llu",&number);
        pthread_mutex_unlock(&lock);
        
        if(bytesRead != EOF)
            print_prime_factors(number);
        else
            break;
   
    }
    
    return;
}
int main()
{
    
//    uint64_t i = 92;
//    if(is_prime(i) == 1){
//        printf("Le nombre %llu est premier\n",i);
//    }else{
//        printf("Le nombre %llu n'est pas premier \n",i);
//    }
    
//    print_prime_factors(84);
    
    open_file_and_find_prime_factors_workerthread();
    
}
