#include <iostream>
#include <fstream>
#include <bits/stdc++.h> 

using namespace std;

struct process{
    public:
        int process_id;
        int p_time;
        int period;
        int iteration_left;

        double priority;

        int curr_start;
        int curr_deadline;
        int cpu_execution;


        int missed_deadline;
        int completed_deadline;
        int waiting_time;

        int is_current;

        int is_ready;

        process()
        {
            this->process_id = -1;
        }


        process(int p_id,int t,int p,int k)
        {
            this->process_id=p_id;
            this->p_time=t;
            this->period=p;
            this->iteration_left=k;

            this->priority = 1/(double)p;
            this->is_current= 0;
            this->is_ready =1;


            curr_deadline = 0+p;
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
            this->is_ready = 1;
        }

        void miss()
        {
            this->curr_deadline += this->period;
            this->curr_start += this->period;

            this->missed_deadline ++;

            this->cpu_execution = p_time;
            this->waiting_time += p_time;

            this->iteration_left--;
            this->is_ready = 1;

        }

};


bool compare(process a, process b)
{
    if(a.priority != b.priority)
        return a.priority > b.priority;
    else
        return a.process_id > b.process_id;
}

int get_min_process_start_time(vector <process> p,int t)
{
    for(int i=0;i<p.size();i++)
            if(p[i].curr_start <=t )
                return i;
    return -1;
}

void fill_log(int strt_idx,int end_idx,int*arr,int val)
{
    for (int i = strt_idx; i <= end_idx; i++)
        arr[i] = val; 
}

int find_idx(vector <process> p,process x)
{
    for (int i = 0; i < p.size(); i++)
        if (p[i].process_id == x.process_id)
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
        k_flag,
        was_running=1;
    
    for(int t=0;true;t++)
    {
        
        p_idx = get_min_process_start_time(processes,t);

        if(p_idx != -1)
        {
            if(!was_running)
                cout << "CPU Idle till :"<<t<<"\n";
            if(last_idx == -1 || !was_running)
                cout <<"Process P"<< processes[p_idx].process_id <<" starts execution at time " <<t<< "\n";
            else
            {
                if(last_idx != p_idx)
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
            last_idx = p_idx;
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
            if(processes[i].curr_deadline < t)
                processes[i].miss();
            if(processes[i].iteration_left>0)
                k_flag=1;
            if(i!=p_idx)
                processes[i].waiting_time ++;
        }
        if(!k_flag)
            break;    
    }

    for(int i=0;i<n;i++)
    {
        cout << "Process P"<<processes[i].process_id <<" Waiting time: "<<processes[i].waiting_time << " Missed Deadlines: " <<processes[i].missed_deadline<< " Completed Deadlines: "<< processes[i].completed_deadline <<" \n";
    }
    return 0;

}