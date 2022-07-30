//Diffie-Hellman key exchange
//Ashley Foley 19178703
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<unistd.h>
#include<stdbool.h>
#include<stdlib.h>

// Power function to return value of a ^ b mod P
unsigned long long int power(unsigned long long int a, unsigned long long int b, unsigned long long int P)
{
    if (b == 1)
        return a;
 
    else
        return (((unsigned long long int)pow(a, b)) % P);
}

bool isPrime(int n)
{
    //Outlier cases
    if (n <= 2)
    {
        return false;
    }
    if (n <= 3)
    {
        return true;
    }

    //This is checked so we can skip these below
    if (n % 2 == 0 || n % 3 == 0)
    {
        return false;
    }
 
    int i = 5;
    while(i * i <= n){
        if (n % i == 0)
        {
            return false;
        }
        if (n % (i + 2) == 0){
            return false;
        }
        i = i + 6;
    }
    return true;
}

void swap(unsigned long long int* x, unsigned long long int* y)
{
    unsigned long long int temp = *x;
    *x = *y;
    *y = temp;
}

void sortArray(unsigned long long int array[], unsigned long long int length)
{

    unsigned long long int i,j, min;
    for(i=0; i<length-1; ++i){
    min = i;
    for(j=i+1; j<length; ++j)
    {
        if(array[j] < array[min])
        {
     min = j;
        }
    }
    swap(&array[min], &array[i]);
    }

}


//Driver program
int main()
{
    unsigned long long int P, g, sharedkey_a, p_key_a, sharedkey_y, p_key_b, secret_key_a, secret_key_b;
    srand(time(0));
    P = rand() % 40;
    while (!isPrime(P))
    {
        P = rand() % 40;
    }

    printf("The value of P : %lld\n", P);

    bool flag = false;
    while(!flag)
    {
     g = (rand() % P);
    while(g <= 1)
    {
         g = (rand() % P);
    }

        unsigned long long int actual_set[P];
        for (int i = 1; i < P; ++i)
        {
            actual_set[i-1] = power(g, i, P);
        }
    
    sortArray(actual_set, P-1);

        //check the set contains each number we need
        bool actual_set_flag = true;
        for (int i = 1; i < P; ++i)
        {
            if (actual_set[i-1] != i)
            {
                actual_set_flag = false;
                break;
            }
        }
        if (actual_set_flag == true)
        {
            flag = true;
        }
    }
 

    int pipefds1[2];
    int pipefds2[2];
    if(pipe(pipefds1) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if(pipe(pipefds2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int pid1 = fork();
    int pid2 = -1;
    if(pid1 != 0)
    {
        pid2 = fork();
    }

    if (pid1 == 0)
    {
        //child 1
    srand(time(0) + getpid());
    p_key_a = (rand() % P);
    while(p_key_a <= 1)
    {
         p_key_a = (rand() % P);
    }
    printf("Private key a %lld*\n", p_key_a);
    
        unsigned long long int send = power(g, p_key_a, P);
    printf("send key a %lld*\n", send);
        write(pipefds1[1], &send, sizeof(send));

        unsigned long long receive;
        read(pipefds2[0], &receive, sizeof(receive));
    printf("receive key a %lld*\n", receive);
        printf("The secret key for Alice is: %lld\n", power(receive, p_key_a, P));
    return 0;
    }
    if (pid2 == 0)
    {
        //child 2
    srand(time(0) + getpid());
    p_key_b = (rand() % P);
    while(p_key_b <= 1)
    {
         p_key_b = (rand() % P);
    }
    printf("Private key b %lld*\n", p_key_b);
        unsigned long long send = power(g, p_key_b, P);
    printf("send key b %lld*\n", send);
        write(pipefds2[1], &send, sizeof(send));

        unsigned long long receive;
        read(pipefds1[0], &receive, sizeof(receive));
    printf("receive key b %lld*\n", receive);
        printf("The secret key for Bob is: %lld\n", power(receive, p_key_b, P));
    return 0;
    }
    return 0;
}