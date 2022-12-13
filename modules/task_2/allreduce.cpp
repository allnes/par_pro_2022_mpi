#include "allreduce.h";

double allR::allreduce(double *sendbuf, double *recvbuf, int count)
{
    MPI_Allreduce(sendbuf, recvbuf, count, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
}

//Функция-обертка вокруг MPI_Allreduce
//sendbuf - Буфер отправки, содержащий количество удвоений (вход)
//recvbuf - Предварительно выделенный приемный буфер (выходной)
//count - Количество элементов в буферах отправки и приема
double allR::allreduceRSAG(double *sendbuf, double *recvbuf, int count)
{
    int thisProc, nProc;
    MPI_Comm_rank( MPI_COMM_WORLD, &thisProc );
    MPI_Comm_size( MPI_COMM_WORLD, &nProc );
    MPI_Status status;

    int dest;
    int width;
    double *myData = new double[count];
    for (int i=0; i<count; i++)
        myData[i]=sendbuf[i];

    double *temp_sendbuf = new double[count];
    for (int i=0; i<count; i++)
        temp_sendbuf[i]=sendbuf[i];

    int nPhases = log2(nProc);

    for (int p = 0; p<nPhases; p++){
        width = (count/pow(2, p+1));

        if (thisProc%((int)pow(2, p+1))<(int)pow(2, p+1)/2){
            dest = thisProc + pow(2,p);
            for (int i=0; i<width; i++)
                temp_sendbuf[i]=myData[i+width]; // вторая половина сообщения
            MPI_Send(temp_sendbuf, width, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
            MPI_Recv(recvbuf, width, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD, &status);

            for (size_t i = 0; i < width; i++)
                myData[i] += recvbuf[i]; // суммируем повтор
        }
        else{
            dest = thisProc - pow(2,p);

            MPI_Recv(recvbuf, width, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD, &status);


            for (int i=0; i<width; i++)
                temp_sendbuf[i]=myData[i]; // первая половина сообщения

            for (size_t i = 0; i < width; i++)
                myData[i+width] += recvbuf[i];
            MPI_Send(temp_sendbuf, width, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);

            // Копируем вторую половину на первую половину
            for (size_t i = 0; i < width; i++)
                myData[i]=myData[i+width];
        }
    }
    for (int p = nPhases-1; p>=0; p--){
        width = (count/pow(2, p+1));
        if (thisProc%((int)pow(2, p+1))<(int)pow(2, p+1)/2){
            dest = thisProc + pow(2,p);
            for (int i=0; i<width; i++)
                temp_sendbuf[i]=myData[i];
            MPI_Send(temp_sendbuf, width, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
            MPI_Recv(recvbuf, width, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD, &status);

            for (size_t i = 0; i < width; i++)
                myData[i+width] = recvbuf[i];
        }
        else{
            dest = thisProc - pow(2,p);

            MPI_Recv(recvbuf, width, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD, &status);

            for (int i=0; i<width; i++)
                temp_sendbuf[i]=myData[i];


            for (size_t i = 0; i < width; i++)
                myData[i+width] = myData[i];

            for (size_t i = 0; i < width; i++)
                myData[i] = recvbuf[i];
            MPI_Send(temp_sendbuf, width, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);

        }
    }

    for (size_t i = 0; i < count; i++)
        recvbuf[i] = myData[i];

    delete [] myData;
    delete [] temp_sendbuf;

}