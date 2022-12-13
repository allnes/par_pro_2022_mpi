#include "allreduce.h"

using namespace std;

int main(int argc, char* argv[])
{
    int thisProc, nProc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &thisProc);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    // инициализируем stndbuf
    double *sendbuf = new double[nProc];

    for (size_t i = 0; i < nProc; i++)
        sendbuf[i] = (i+1)*(thisProc+1);

    // инициализируем recvbuf
    double *recvbuf = new double[nProc];

    allR::allreduceRSAG(sendbuf, recvbuf, nProc);

    //Тест
    double *recvbufGround = new double[nProc];
    allR::allreduce(sendbuf, recvbufGround, nProc);
    allR::allreduce(sendbuf, recvbuf, nProc);
    bool test = true;
    for (int i=0; i<nProc; i++){
        if (recvbuf[i]!=recvbufGround[i])
            test=false;
    }
    if (test)
        cout<<"Test passed"<<endl;
    else
        cout<<"Test failed"<<endl;

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}