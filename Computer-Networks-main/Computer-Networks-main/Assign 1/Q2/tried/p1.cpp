#include <bits/stdc++.h>
using namespace std;


void initialConnection(int writeFD){
	FILE* ptr[2] = {stdin,stdout};
	write(writeFD,ptr,sizeof(ptr)+1);
	return;
}

int main(){
	int pid,pfd1[2],pfd2[2];
	if(pipe(pfd1) == -1)return -1;
	if(pipe(pfd2) == -1)return -1;
	
	FILE* rptr1 = fdopen(pfd1[0],"r");
	FILE* wptr1 = fdopen(pfd1[1], "w");
	FILE* rptr2 = fdopen(pfd2[0],"r");
	FILE* wptr2 = fdopen(pfd2[1], "w");
	
	FILE* fp = stdin;
	char proclnk[0xFFF];
	char filename[0xFFF];
	
    	int MAXSIZE = 0xFFF;
	int fno = fileno(fp);
        sprintf(proclnk, "/proc/self/fd/%d", fno);
        size_t r = readlink(proclnk, filename, MAXSIZE);
        
        if (r < 0)
        {
            printf("failed to readlink\n");
            exit(1);
        }
        filename[r] = '\0';
        printf("fp -> fno -> filename: %p -> %d -> %s\n", fp, fno, filename);
        
	
	cout << stdin << " " << stdout << endl;
	cout << rptr1 << " " << wptr1 << endl;
	cout << rptr2 << " " << wptr2 << endl;
	if((pid = fork()) == -1)return -1;
	
	if(pid){
		close(pfd1[0]);
		close(pfd2[1]);
		
		//initialConnection(pfd1[1]);
		
		write(pfd1[1], filename,strlen(filename)+1);
		
		
		
		int BUFFER_LEN = 10000;
		char s[BUFFER_LEN];
		//if(read(pfd2[0],s,BUFFER_LEN))
		//		cout << "From P2 : " << s << " lol " << endl;
		/*while(1){
			if(read(pfd2[0],s,BUFFER_LEN))
				cout << "From P2 : " << s << endl;
			else
				break;
			cout << "P1 Enter : ";
			if(cin.getline(s,BUFFER_LEN))
				write(pfd1[1],s,strlen(s)+1);
			else
				break;
			cout << "loop end p1 \n";
		}*/
		cout << "p1 exit\n";
		close(0);
		close(pfd1[1]);
		close(pfd2[0]);
	}
	else{
		dup2(pfd1[0],0);
		dup2(pfd2[1],1);
		system("g++ p2.cpp -o p2");
        	execv("./p2",NULL);
		
	}
	return 0;
}	
		
		
