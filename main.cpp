#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

bool f(int);
bool g(int);

int main(int argc, char** argv)
{
    int child1_to_parent[2];
    int child2_to_parent[2];
    int parent_to_child1[2];
    int parent_to_child2[2];
    bool res1, res2;
    int val;
    int status1 = 0, status2 = 0;

    pipe(child1_to_parent);
    pipe(child2_to_parent);
    pipe(parent_to_child1);
    pipe(parent_to_child2);

    pid_t firstChild, secondChild, wpid; // идентификатор процесса или группы процессов

    int x;
    printf("Enter data:\n");
    scanf("%d", &x);


    if((firstChild = fork()) < 0) //породжуємо новий процес (процес-син)
        // case -1: за виклику fork() виникла помилка
        // case 0 : код процесу сина
        // default : код батьківсього процесу
    {
        perror("Can't fork process");
        exit(EXIT_FAILURE);
    }

    if(firstChild == 0)
    {
        read(parent_to_child1[0],&val, sizeof(val));
        // parent_to_child[0] - дескриптор файла для читання
        // &val - куди записує
        // sizeof(val) - скільки ми хочемо прочитати
        printf("Child 1 received %d\n", val);
        res1 = f(val);
        write(child1_to_parent[1], &res1, sizeof(res1));
        // child_to_parent[1] - дескриптор файла для запису
        printf("Child 1 send %d\n", res1);
        exit(EXIT_SUCCESS);
    }
    else // Parent йде в else
    {
        write(parent_to_child1[1], &x, sizeof(x));
        write(parent_to_child2[1], &x, sizeof(x));

        if((secondChild = fork()) < 0)
        {
            perror("Can't fork process");
            exit(EXIT_FAILURE);
        }

        if(secondChild == 0)
        {
            read(parent_to_child2[0],&val, sizeof(val));
            printf("Child 2 received %d\n", val);
            res2 = g(val);
            write(child2_to_parent[1], &res2, sizeof(res2));
            printf("Child 2 send %d\n", res2);
            exit(EXIT_SUCCESS);
        }
        else
        {
            int time = 1;
            bool firstComplete = false;
            bool secondComplete = false;
            while (true)
            {
                pid_t firstCheck = waitpid(firstChild, &status1, WNOHANG);
                // WNOHANG - означає миттєве повернення управління, якщо жоден дочірній процесс не завершив виконання
                pid_t secondCheck = waitpid(secondChild, &status2, WNOHANG);

                std::cout << firstCheck << ' ' << secondCheck << std::endl;

                if(firstCheck > 0) firstComplete = true;
                if(secondCheck > 0) secondComplete = true;

                if(firstComplete && secondComplete)
                {
                    break;
                }

                if(time % 5 == 0)
                {
                    printf("Do you want to continue calculations: y\\n?\n");
                    char response;
                    scanf(" %c", &response);
                    if(response != 'y')
                    {
                        kill(firstChild, SIGKILL);
                        kill(secondChild, SIGKILL);
                        return 0;
                    }
                }
                sleep(1);
                //usleep(1);
                time++;
            }
            read(child1_to_parent[0], &res1, sizeof(res1));
            read(child2_to_parent[0], &res2, sizeof(res2));

            printf("f(x)=%d and g(x)=%d\n", res1, res2);
            printf("Result: %d * %d = %d\n", res1, res2, res1 * res2);
        }
    }
}

bool f(int x)
{
    return x > 0;
}

bool g(int x)
{
    if (x == 0)
    {
        while(true);
    }
    return (x * x - x) > 1;
}