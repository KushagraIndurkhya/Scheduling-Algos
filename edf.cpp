#include <iostream>
#include <fstream>
#include <bits/stdc++.h> 

using namespace std;

struct process{
    public:
        //meta
        int process_id;
        int p_time;
        int period;
        int repetition;
        

        double priority;

        //helper
        int iteration_left;
        int curr_start;
        int curr_deadline;
        int cpu_execution;

        //stats
        int missed_deadline;
        int completed_deadline;
        int waiting_time;

        //Constructor
        process(int p_id,int t,int p,int k)
        {
            this->process_id=p_id;
            this->p_time=t;
            this->period=p;
            this->repetition = k;
            

            this->priority = 1/(double)p;

            curr_deadline = 0+p;
            iteration_left=k;
            curr_start = 0;
            cpu_execution = p_time;

            missed_deadline = 0;
            completed_deadline=0;
        }

        void complete_iteration()
        {
            this->curr_deadline += this->period;
            this->curr_start += this->period;

            this->completed_deadline++;

            this->cpu_execution = p_time;

            this->iteration_left--;
        }

        void miss(int t)
        {
            
    
            this->missed_deadline ++;

            this->waiting_time -= t-curr_start-(p_time-cpu_execution);
            this->waiting_time += period;

            this->curr_deadline += this->period;
            this->curr_start += this->period;
            this->cpu_execution = p_time;

            this->iteration_left--;
        }
};


bool compare(process a, process b)
{
    if(a.curr_deadline != b.curr_deadline)
        return a.curr_deadline < b.curr_deadline;
    else
        return a.process_id < b.process_id;
}

int get_min_process_start_time(vector <process> p,int t)
{
    
    for(int i=0;i<p.size();i++)
            if(p[i].curr_start <=t && p[i].iteration_left > 0 )
                return p[i].process_id;
    return -1;
}

int find_idx(vector <process> p,int id)
{
    for(int i=0;i<p.size();i++)
            if(p[i].process_id == id)
                return i;
    return -1;
}

int main()
{
    int n,p_id,ti,p,k;
    cin >> n;

    vector <process> processes;
    
    for (int i = 0; i < n; i++)
    {
        cin >> p_id >> ti >> p >> k ;
        processes.push_back(*(new process(p_id,ti,p,k))) ;
    }
    sort(processes.begin(),processes.end(),compare);
    //...........................................................................
    int last_idx= -1,p_idx,
        c_id=-1,l_id=-1,
        k_flag,
        was_running=1;

    // for (int i = 0; i < n; i++)
    // {
    //     cout<<processes[i].process_id<<"...\n";
    // }
    
    for(int t=0;true;t++)
    {
        sort(processes.begin(),processes.end(),compare);
        c_id =get_min_process_start_time(processes,t);
        p_idx = find_idx (processes,c_id);
        last_idx = find_idx (processes,l_id);
        

        // cout << "$$$$" <<p_idx<<"*********"<<last_idx<<"\n";

        if(p_idx != -1)
        {
            if(!was_running)
                cout << "CPU Idle till :"<<t<<"\n";
            if(last_idx == -1 || !was_running)
                cout <<"Process P"<< processes[p_idx].process_id <<" starts execution at time " <<t<< "\n";
            else
            {
                if(p_idx != last_idx)
                {
                    if(processes[last_idx].cpu_execution == processes[last_idx].p_time )
                    {
                        if(processes[p_idx].cpu_execution == processes[p_idx].p_time)
                            cout <<"Process P"<< processes[p_idx].process_id <<" starts execution at time " <<t<< "\n";
                        else
                            cout <<"Process P"<< processes[p_idx].process_id <<" resumes execution at time " <<t<< "\n";
                    }
                    else
                    {
                        cout <<"Process P"<< processes[last_idx].process_id <<" preempted by Process P" <<processes[p_idx].process_id <<" at time " <<t<< ". Remainig processing time:"<<processes[last_idx].cpu_execution<<"\n";
                        cout <<"Process P"<< processes[p_idx].process_id <<" starts execution at time " <<t<< "\n";
                    }
                }
            }
            
            processes[p_idx].cpu_execution--;
            if(processes[p_idx].cpu_execution==0)
            {   
                processes[p_idx].complete_iteration();
                cout <<"Process P"<< processes[p_idx].process_id  <<" finishes execution at time " <<t+1 << "\n";
            }
            l_id = c_id;

            was_running=1;
        }
        else
        {
            was_running=0;
            continue;
        }
        k_flag=0;
        
        for(int i=0;i<n;i++)
        {
            if(processes[i].iteration_left > 0 && t+processes[i].cpu_execution > processes[i].curr_deadline)
            {
                processes[i].miss(t);
                // p_idx = find_idx (processes,c_id);
                // sort(processes.begin(),processes.end(),compare);
                cout <<"Process P"<< processes[i].process_id  <<" missed deadline at time " <<t << "\n";
            }
            if(processes[i].iteration_left>0)
                k_flag=1;
        }
        if(!k_flag)
            break; 
        p_idx = find_idx (processes,c_id);
        for(int i=0;i<n;i++)
        {
            if(
                i!=p_idx 
                && processes[i].curr_start < t+1
                && processes[i].iteration_left >0
            )
            {
            //     if(processes[i].process_id == 1)
            //     {
            //         cout <<i<<p_idx<<"..........."<< processes[i].curr_start << "......" << processes[i].curr_deadline << ".........." << t << "......" << processes[i].cpu_execution <<"\n"  ;
            // }
            processes[i].waiting_time ++;
        }
        }
           
    }

    double avg = 0.0;

    for(int i=0;i<n;i++)
    {
        cout << "Process P"<<processes[i].process_id <<" Waiting time: "<<processes[i].waiting_time << " Missed Deadlines: " <<processes[i].missed_deadline<< " Completed Deadlines: "<< processes[i].completed_deadline;
        cout << " Avg Waiting Time: "<<processes[i].waiting_time/(double)processes[i].repetition <<" \n";

        avg += processes[i].waiting_time/(double)processes[i].repetition;
    }

    cout << "Avg_waiting_time: " << avg/n <<"\n";
    return 0;

}