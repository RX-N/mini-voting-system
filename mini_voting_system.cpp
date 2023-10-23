#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;

struct Candidate
{
    string name;
    int votes;
    Candidate(string name) : name(name), votes(0) {}
};

void printTextLetterByLetter(const string &text, int delay)
{
    for (char c : text)
    {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
}

void printSplitTerminal(const vector<Candidate> &candidates)
{
    int totalCandidates = candidates.size();

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < totalCandidates; j++)
        {
            if (i == 0)
            {
                cout << j + 1 << ". " << candidates[j].name << " , votes: " << candidates[j].votes << " | ";
            }
        }
        cout << endl;
    }
}

void clearLines(int numLines)
{
    for (int i = 0; i < numLines; i++)
    {
        cout << "\x1b[A"; // Move the cursor up one line
        cout << "\x1b[K"; // Clear the line
    }
}

int votingProcess(vector<Candidate> &candidates, queue<int> &votesQueue)
{
    int candidateNumber;

    printTextLetterByLetter("Menu for Voting:\n", 60);

    for (int i = 0; i < candidates.size(); i++)
    {
        cout << i + 1 << ". " << candidates[i].name << endl;
    }

    printTextLetterByLetter("Enter the \033[33mnumber of the candidate\033[0m you want to vote for (or 0 to exit): ", 70);
    cin >> candidateNumber;

    if (candidateNumber == 0)
    {
        printTextLetterByLetter("\033[31mExit\033[0m\n", 60);
        return 0;
    }

    if (candidateNumber >= 1 && candidateNumber <= candidates.size())
    {
        votesQueue.push(candidateNumber - 1);
        printTextLetterByLetter("Your vote for \033[33m" + candidates[candidateNumber - 1].name + "\033[0m has been recorded.\n", 70);
        return 1;
    }
    else
    {
        printTextLetterByLetter("\033[31mInvalid candidate number. Your vote is not recorded.\033[0m\n", 70);
        return -1;
    }
}

int main()
{
    vector<Candidate> candidates;
    queue<int> votesQueue;

    int totalCandidate;
    string candidateName;

    printTextLetterByLetter("Enter number of candidates (max 10): ", 70);
    cin >> totalCandidate;

    if(totalCandidate > 0 && totalCandidate <=10 ) {

        for (int i = 1; i <= totalCandidate; i++)
        {
            printTextLetterByLetter("Enter the name of candidate number " + to_string(i) + ": ", 70);
            cin >> candidateName;
            candidates.push_back(Candidate(candidateName));
        }

        cout << "\033[2J\033[H"; // Clear the terminal screen

        while (true)
        {
            cout << "\033[33m----- Voting System -----\033[0m" << endl;

            int choice = 1;
            while (choice != 0)
            {
                choice = votingProcess(candidates, votesQueue);
                clearLines(candidates.size() + 3); // Clear the lines of the last vote
            }

            // Process votes
            while (!votesQueue.empty())
            {
                int candidateNumber = votesQueue.front();
                candidates[candidateNumber].votes++;
                votesQueue.pop();
            }

            cout << "\033[2J\033[H"; // Clear the terminal screen

            cout << "\033[33m----- Voting System -----\033[0m" << endl;
            printTextLetterByLetter("Do you want to go back? (y/n): ", 60);
            char c;
            cin >> c;
            if (c == 'y' || c == 'Y')
            {
                cout << "\033[2J\033[H"; // Clear the terminal screen
                continue;
            }
            else
            {
                clearLines(1);
                printSplitTerminal(candidates);
                printTextLetterByLetter("Do you want to save the votes to a file? (y/n): ", 70);
                cin >> c;

                if (c == 'y' || c == 'Y')
                {
                    string filename;
                    printTextLetterByLetter("Enter the filename: ", 60);
                    cin >> filename;

                    ofstream file(filename);

                    for (int i = 0; i < candidates.size(); i++)
                    {
                        file << candidates[i].name << ", votes: " << candidates[i].votes << endl;
                    }

                    file.close();
                    printTextLetterByLetter("\033[31mExit\033[0m\n", 60);
                    break;
                }
                else
                {
                    printTextLetterByLetter("\033[31mExit\033[0m\n", 60);
                    break;
                }
            }
        }
    } else {
        printTextLetterByLetter("\033[31mInvalid number of candidates. Please restart the program.\033[0m\n", 70);
    }

    return 0;
}
