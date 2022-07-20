#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>

using namespace std;

const string input = "\\input{";

void countWords(string fileName, string word, int &count)
{
    ifstream file(fileName);
    string line;
    int childrenCounter = 0;
    int pid;
    int status;

    while (getline(file, line))
    {
        if (line.find(input) != std::string::npos)
        {
            pid = fork();
            childrenCounter++;

            if (pid == 0)
            {
                count = 0;
                
                string newFileName = line.substr(input.length()-1, line.length());
                newFileName.pop_back();
                newFileName.erase(0, 1);
                newFileName.erase(newFileName.size() - 1);
                //cout << newFileName;

                countWords(newFileName, word, count);

                exit(count);
            }
        }

        string::size_type pos = 0;
        while ((pos = line.find(word, pos)) != std::string::npos)
        {
            count++;
            pos += word.length();
        }
    }

    if (pid > 0)
    {
        for (int i = 0; i < childrenCounter; i++)
        {
            wait(&status);
            count += WEXITSTATUS(status);
        }
    }
}

int main(int argc, char *argv[])
{
    string fileName = argv[1], word = argv[2];

    int count = 0;
    
    countWords(fileName, word, count);
    cout << count << endl;

    return 0;
}