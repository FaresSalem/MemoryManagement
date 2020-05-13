#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

typedef struct hole
{
    size_t base;
    size_t limit;
}Hole;

typedef struct segment
{
    string segment_name;
    size_t limit;
}Segment;

typedef struct process
{
    size_t process_id;
    vector <Segment> seg_list;
}Process;


vector <hole> hole_list;                           //data structure to store information of holes in memory
vector <Process> process_list;                     //data structure to store information of processes in memory
vector <int> memory;      


void initialize_memory(size_t mem_size)
{
    for (size_t i = 0; i < mem_size;i++)
    {
        memory.push_back(-1);
    }
}
void assign_hole(size_t base, size_t limit)
{
    static int processesCount = 1;

    hole_list.push_back({base,limit });
    
    for (size_t j = base;j < base + limit;j++)
        memory[j] = 0;

    if (memory[base - 1] == -1)
    {
        for (int i = base - 1;i >= 0 && memory[i] == -1;i--)
            memory[i] = processesCount;
        
        processesCount++;
    }
    if (memory[base + limit] == -1 || memory[base + limit] > 0)
    {
        for (size_t i = base + limit;i < memory.size() && (memory[i] > 0 || memory[i] == -1);i++)
            memory[i] = processesCount;

        processesCount++;
    }
}
void assign_process(size_t process_id, size_t num_seg)
{
    vector<Segment> seg_list;
    string segment_name;
    size_t limit;
    
    for (size_t i = 0;i < num_seg;i++)
    {
        cout << "enter segment name and limit of segment " << i + 1 << endl;
        cin >> segment_name;
        cin >> limit;

        Segment s1 ={segment_name,limit};
        seg_list.push_back(s1);
    }

    process_list.push_back({ process_id,seg_list });
}


//to free locations in memory//
void free(size_t process_id, size_t mem_size)
{
    for (size_t i = 0; i < mem_size; i++)
    {
        if (memory[i] == process_id)
            memory[i] = 0;
    }
}


/*now we want to allocate processes in the memory using best fit algorithm ,okay we have a vector that we store processes in it so we will
loop on this vector and each process we will take it and try to allocate it in the memory,if it is allocated successfully we will erase it from
the vector and if it isn't .. we will keep it in the vector and go to the next process and we still loop until all processes finish....there is
something important if any segment of the process can't be allocated we will pend the whole process not the segment only so we will work on
secondary vector for memory and fill it to the primary vector that gui work with at the end of the alloction of the process */

vector <int> sec_mem;                       

bool best_fit()
{
    int flag = 0;
    for (int i = 0; i < memory.size();i++)
    {
        sec_mem.push_back(memory[i]);
    }
    Hole h1;
    h1.limit = 9999999;
    static int i = 0;
    int hole_index = 0;
    for (int j = 0;j < process_list[i].seg_list.size();j++)
    {
        for (int k = 0;k < hole_list.size();k++)
        {
            if (process_list[i].seg_list[j].limit <= hole_list[k].limit)
            {

                if (h1.limit > hole_list[k].limit)
                {
                    h1 = hole_list[k];
                    hole_index = k;
                }
            }

        }
        if (h1.limit != 9999999)
        {
            flag++;
           
            for (int s = hole_list[hole_index].base; s < hole_list[hole_index].base+ process_list[i].seg_list[j].limit;s++)
            {
                sec_mem[s] = process_list[i].process_id;
            }
            hole_list[hole_index].limit -= process_list[i].seg_list[j].limit;
            hole_list[hole_index].base += process_list[i].seg_list[j].limit;
        }
        else
        {
            cout << "no" << endl;
            return 1;
        }
            
            
    }

    if (flag == process_list[i].seg_list.size())
    {
        for (int m = 0;m < memory.size();m++)
            memory[m] = sec_mem[m];
    }
    i++;
    return 1;
}


// we need to check this code 
int main()
{
    size_t size_of_mem, num_of_holes;

    cout << "enter size of memory: ";
    cin >> size_of_mem;

    initialize_memory(size_of_mem);

    cout << "enter number of holes: ";
    cin >> num_of_holes;

    size_t base, limit;

    for (int i = 0; i < num_of_holes; i++)
    {
        cout << "enter base and limit of hole " << i + 1 << endl;
        cin >> base;
        cin >> limit;

        assign_hole(base,limit);
    }
    
    size_t num_seg, process_id, p_num;

    cout << "enter number of processes: ";
    cin >> p_num;

    for (size_t i = 0;i < p_num;i++)
    {
        cout << "enter process id and number of segment of this process \n ";
        cin >> process_id;
        cin >> num_seg;
        assign_process(process_id, num_seg);
        best_fit();
    }

    for (int i = 0;i < size_of_mem;i++)
    {
        cout << memory[i] << "    "  << i << endl;

    }


/*
    for (int i = 0;i < process_list.size();i++)
    {
        cout << process_list[i].process_id << endl;
        cout << process_list[i].seg_list[i].limit << endl;
        cout << process_list[i].seg_list[i].segment_name << endl;

    }
*/
    return 0;
}
