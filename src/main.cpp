#include "processcontainer.h"
#include "sysinfo.h"
#include "curses.h"

char* getCString(std::string s) {
    char * cstr = new char [s.length()+1];
    strcpy (cstr, s.c_str());
    return cstr;
}


void writeSysInfoToConsole(SysInfo sys, WINDOW* sys_win)
{
    sys.setAttributes();

    int row{0};
    wattron(sys_win,A_BOLD);
    mvwprintw(sys_win, row, (sys_win->_maxx/2 - 5), getCString("SYSTEM INFO"));
    mvwprintw(sys_win, ++row, 2, getCString(""));
    mvwprintw(sys_win,++row,2,getCString( "OS: "));
    wattron(sys_win,COLOR_PAIR(1));
    mvwprintw(sys_win,row,14, getCString(sys.getOsName()));
    wattroff(sys_win,COLOR_PAIR(1));
    mvwprintw(sys_win,++row,2,getCString( "Kernel ver: "));
    wattron(sys_win,COLOR_PAIR(1));
    mvwprintw(sys_win,row,14, getCString(sys.getKernelVersion()));
    wattroff(sys_win,COLOR_PAIR(1));
    mvwprintw(sys_win,++row,2,getCString( "CPU: "));
    wattron(sys_win,COLOR_PAIR(5));
    mvwprintw(sys_win,row, 11, getCString(Util::getProgressBar(sys.getCpuPercent())));
    wattroff(sys_win,COLOR_PAIR(5));
    mvwprintw(sys_win,++row,2,getCString(( "Other cores:")));
    wattron(sys_win,COLOR_PAIR(5));
    mvwprintw(sys_win, ++row, 2, getCString(""));
    std::vector<std::string> val = sys.getCoresStats();
    for (std::size_t i = 0; i < val.size(); i++) {
        mvwprintw(sys_win,(row+i),5,getCString(val[i]));
    }
    wattroff(sys_win,COLOR_PAIR(5));
    mvwprintw(sys_win,13,2,getCString(( "Memory:  ")));
    wattron(sys_win,COLOR_PAIR(5));
    wprintw(sys_win,getCString(Util::getProgressBar(sys.getMemPercent())));
    wattroff(sys_win,COLOR_PAIR(5));
    mvwprintw(sys_win,14,2,getCString(( "[Processes]" )));
    mvwprintw(sys_win,15,2,getCString(( " -Total:" )));
    wattron(sys_win,COLOR_PAIR(1));
    mvwprintw(sys_win,15,14, getCString(sys.getTotalProc()));
    wattroff(sys_win,COLOR_PAIR(1));
    mvwprintw(sys_win,16,2,getCString(( " -Running:")));
    wattron(sys_win,COLOR_PAIR(1));
    mvwprintw(sys_win,16,14, getCString(sys.getRunningProc()));
    wattroff(sys_win,COLOR_PAIR(1));
    mvwprintw(sys_win,17,2,getCString("Up Time: "));
    wattron(sys_win,COLOR_PAIR(1));
    mvwprintw(sys_win,17,14, getCString(Util::convertToTime(sys.getUpTime())));
    wattroff(sys_win,COLOR_PAIR(1));
    wattroff(sys_win,A_BOLD);

}


void getProcessListToConsole(ProcessContainer procs,WINDOW* win)
{
    procs.refreshList();
    int row{0};
    wattron(win,A_BOLD);
    mvwprintw(win, row, (win->_maxx/2 - 6), getCString("PROCESS LIST"));
    wattroff(win,A_BOLD);
    mvwprintw(win, ++row, 2, getCString(""));
    int const pid_column{2};
    int const user_column{9};
    int const cpu_column{21};
    int const ram_column{31};
    int const time_column{40};
    int const command_column{51};
    wattron(win, COLOR_PAIR(4));
    wattron(win, A_BOLD);
    std::string filler = std::string(100, ' ');
    std::string title = "PID    USER        CPU[%]    RAM[MB]  UPTIME     COMMAND" +  filler;
    std::string titleText = title.substr(0, win->_maxx -3);
    mvwprintw(win, ++row, pid_column, getCString(titleText));
    wattroff(win, A_BOLD);
    wattroff(win, COLOR_PAIR(4));
    std::vector<std::vector<std::string>> processes = procs.getList();
    for(int i = 0; i < 10; i++) {
        std::vector<std::string> aprocess = processes[i];
        mvwprintw(win, ++row, pid_column, getCString(aprocess[0]));
        mvwprintw(win, row, user_column, getCString(aprocess[1]));
        mvwprintw(win, row, cpu_column, getCString(aprocess[2]));
        mvwprintw(win, row, ram_column, getCString(aprocess[3]));
        mvwprintw(win, row, time_column, getCString(aprocess[4]));
        mvwprintw(win, row, command_column, getCString(aprocess[5]));
    }
}


void printMain(SysInfo sys,ProcessContainer procs)
{
    initscr(); 
    noecho(); 
    cbreak();
    start_color();
    int xMax = getmaxx(stdscr); 
    WINDOW *sys_win = newwin(20,xMax-1,0,0);
    WINDOW *proc_win = newwin(15,xMax-1,21,0);

    init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_WHITE,COLOR_BLUE);
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);
    init_pair(4,COLOR_BLUE,COLOR_WHITE);
    init_pair(5,COLOR_GREEN,COLOR_BLACK);

    while (true) {
        wattron(sys_win, COLOR_PAIR(2));
        wattron(proc_win, COLOR_PAIR(2));
        box(sys_win,0,0);
        box (proc_win,0,0);
        wattroff(sys_win, COLOR_PAIR(2));
        wattroff(proc_win, COLOR_PAIR(2));
        writeSysInfoToConsole(sys,sys_win);
        getProcessListToConsole(procs,proc_win);
        wrefresh(sys_win);
        wrefresh(proc_win);
        refresh();
        sleep(1);
    }
    endwin();
}


int main()
{
    ProcessContainer procs;
    SysInfo sys;
    printMain(sys,procs);
    return 0;
}
