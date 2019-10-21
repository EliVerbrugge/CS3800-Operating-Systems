#include "schedulers.h"
#include "limits.h"
//Round Robin scheduler implementation. In general, this function maintains a double ended queue
//of processes that are candidates for scheduling (the ready variable) and always schedules
//the first process on that list, if available (i.e., if the list has members)
int RoundRobin(const int& curTime, const vector<Process>& procList, const int& timeQuantum)
{
    static int timeToNextSched = timeQuantum;  //keeps track of when we should actually schedule a new process
    static deque<int> ready;  //keeps track of the processes that are ready to be scheduled

    int idx = -1, chk;
    bool done;

    // first look through the process list and find any processes that are newly ready and
    // add them to the back of the ready queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
        }
    }

    // now take a look the head of the ready queue, and update if needed
    // (i.e., if we are supposed to schedule now or the process is done)
    if(timeToNextSched == 0 || procList[ready[0]].isDone)
    {
        // the process at the start of the ready queue is being taken off of the
        // processor

        // if the process isn't done, add it to the back of the ready queue
        if(!procList[ready[0]].isDone)
        {
            ready.push_back(ready[0]);
        }

        // remove the process from the front of the ready queue and reset the time until
        // the next scheduling
        ready.pop_front();
        timeToNextSched = timeQuantum;
    }

    // if the ready queue has any processes on it
    if(ready.size() > 0)
    {
        // grab the front process and decrement the time to next scheduling
        idx = ready[0];
        --timeToNextSched;
    }
    // if the ready queue has no processes on it
    else
    {
        // send back an invalid process index and set the time to next scheduling
        // value so that we try again next time step
        idx = -1;
        timeToNextSched = 0;
    }

    // return back the index of the process to schedule next
    return idx;
}

//Shortest Process Next scheduler implementation. This always schedules the shortest process in the list
//and runs that process until completion
int ShortestProcessNext(const int& curTime, const vector<Process>& procList)
{
    static int timeToNextSched = 0;  //keeps track of when we should actually schedule a new process
    static vector<bool> doneProcesses(procList.size(), false);
    static int currProcess = -1;
    //local vars
    int idx = -1, chk;
    bool done;
    int minTime = 0;

     //////////////////////////////////////////
    //This process picks the smallest task 
    //available and after that task is done
    //looks for the next smallest available task
    /////////////////////////////////////////

    //if we are done with the current process check for the new min process
    if(timeToNextSched <= 0 || procList[currProcess].isDone)
    {
        bool allProcessesDone = true;
        for(int j = 0; j < procList.size(); j++)
        {
            if(doneProcesses[j] == false)
                allProcessesDone = false;
        }
        //make the mintime the max value an int can hold, that way everything will be less
        //if all the processes are done, we can return.
        if(procList.size() > 0 && !allProcessesDone)
        {
            minTime = INT_MAX;
        }
        else
        {
            // send back an invalid process index and set the time to next scheduling
            // value so that we try again next time step
            idx = -1;
            timeToNextSched = 0;
            currProcess = -1;
            return idx;
        }

        if(currProcess != -1)
        {
            doneProcesses[currProcess] = true;
        }

        // if the process is done we look for the shortest process that was made available
        for(int i = 0, i_end = procList.size(); i < i_end; i++)
        {
            if(procList[i].totalTimeNeeded < minTime && curTime >= procList[i].startTime && !doneProcesses.at(i))
            {
                cout << i << endl;
                minTime = procList[i].totalTimeNeeded;
                idx = i;
            }
        }
        //update the current process and time to next task needs to be scheduled, also remove the task we scheduled fromn available tasks.
        cout << idx << endl;
        currProcess = idx;
        timeToNextSched = minTime;
    }
    else
    {
        timeToNextSched--;
        idx = currProcess;
    }

    // return back the index of the process to schedule next
    return idx;
}

//Shortest Remainting Time scheduler implementation. This always schedules the process with the least remaining time
int ShortestRemainingTime(const int& curTime, const vector<Process>& procList)
{
    static int timeToNextSched = 0;  //keeps track of when we should actually schedule a new process
    static vector<bool> doneProcesses(procList.size(), false);
    static int currProcess = -1;
    //local vars
    int idx = -1, chk;
    bool done;
    int minTime = 0;
    bool allProcessesDone = true;
    int timeRemaining = -1;

    //////////////////////////////////////////
    //Similar process to Shortest Process Next
    //the only difference being we always look
    //for the shortest remaining time instead
    //of shortest total time after a process
    //is done
    /////////////////////////////////////////

    //check if all of the processes are done
    for(int j = 0; j < procList.size(); j++)
    {
        if(doneProcesses[j] == false)
        {
            allProcessesDone = false;
        }
    }

    //make the mintime the max value an int can hold, that way everything will be less
    //if all the processes are done, we can return.
    if(procList.size() > 0 && !allProcessesDone)
    {
        minTime = INT_MAX;
    }

    else
    {
        // send back an invalid process index and set the time to next scheduling
        // value so that we try again next time step
        idx = -1;
        timeToNextSched = 0;
        currProcess = -1;
        return idx;
    }

    // we look for the shortest remaining time in a process that was made available
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        timeRemaining = procList[i].totalTimeNeeded - procList[i].timeScheduled;
        doneProcesses[i] = procList[i].isDone; //update the list, so we can be sure of what process is done
        if(timeRemaining < minTime && curTime >= procList[i].startTime && !doneProcesses.at(i))
        {
            minTime = timeRemaining;
            idx = i;
        }
    }

    //update the current process and time to next task needs to be scheduled, also remove the task we scheduled fromn available tasks.
    cout << idx << endl;
    currProcess = idx;
    timeToNextSched = timeRemaining;

    if(timeToNextSched > 0)
    {
        timeToNextSched--;
        idx = currProcess;
    }

    // return back the index of the process to schedule next
    return idx;
}

//Highest Response Ration Next scheduler implementation. This always schedules the process with the greatest response ratio
int HighestResponseRatioNext(const int& curTime, const vector<Process>& procList)
{
    //statics
    static int timeToNextSched = 0;  //keeps track of when we should actually schedule a new process
    static vector<bool> doneProcesses(procList.size(), false);
    static int currProcess = -1;

    //local vars
    int idx = -1, chk;
    bool done;
    float maxRatio = 0;

    //////////////////////////////////////////
    //Similar process to Shortest Process Next
    //the only difference being we compare
    //to the response ration not just shortest
    //execution time
    /////////////////////////////////////////
    
    //if we are done with the current process check for the new min process
    if(timeToNextSched <= 0 || procList[currProcess].isDone)
    {
        bool allProcessesDone = true;
        for(int j = 0; j < procList.size(); j++)
        {
            if(doneProcesses[j] == false)
                allProcessesDone = false;
        }
        //make the mintime the min value an int can hold, that way everything will be more
        if(procList.size() > 0 && !allProcessesDone)
        {
            maxRatio = INT_MIN;
        }
        else
        {
            // send back an invalid process index and set the time to next scheduling
            // value so that we try again next time step
            idx = -1;
            timeToNextSched = 0;
            currProcess = -1;
            return idx;
        }

        if(currProcess != -1)
        {
            doneProcesses[currProcess] = true;
        }

        // if the process is done we look for the highest response ration
        for(int i = 0, i_end = procList.size(); i < i_end; i++)
        {
            int waitingTime = curTime - procList[i].timeScheduled;
            //response ratio is the (waiting time + execution time)/execution time
            float ratio = (waitingTime + procList[i].totalTimeNeeded)/(float)procList[i].totalTimeNeeded;
            if(ratio > maxRatio && curTime >= procList[i].startTime && !doneProcesses.at(i))
            {
                cout << i << endl;
                maxRatio = ratio;
                idx = i;
            }
        }
        //update the current process and time to next task needs to be scheduled, also remove the task we scheduled fromn available tasks.
        cout << idx << endl;
        currProcess = idx;
        timeToNextSched = procList[idx].totalTimeNeeded;
    }
    else
    {
        timeToNextSched--;
        idx = currProcess;
    }

    // return back the index of the process to schedule next
    return idx;
}