#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


double  PSNR(int frameToCompareP2,int frameToCompareP1);

char *path1 = ".../frames%d.yuv"; // address of the reconstructed video frames (YUV frames)
char *path2 = ".../frames%d.yuv"; // address of the original video frames (YUV frames)
//video resolution
int width = 1920;
int height = 1080;
//////////////////

int reconstructed_frame_number = 50;
int original_frame_number = 190;
int alignment_range = 1;

unsigned long long int diffSum = 0;
unsigned long long int diffSumComp = 18446744073709551615;
char address1[500];
char address2[500];
int MostSimilarFrame = 0;
int filelen1 = 0;
int filelen;
int bytes_read;
int bytes_read1;

unsigned char* buffer;
unsigned char* buffer1;
int frameToCompare;

int FirstFrame(int baseFrameNumber, int rangeStart)
{
    sprintf(address1, path1, baseFrameNumber);
    FILE *YUV = fopen(address1, "rb");
    fseek(YUV, 0, SEEK_END);
    filelen = ftell(YUV);
    rewind(YUV);
    buffer = (unsigned char *)malloc(filelen * sizeof(char));
    bytes_read = fread(buffer, filelen,1, YUV);


    FILE *YUV1;
    int i = 0;
    frameToCompare = rangeStart;
    int temp;
    sprintf(address2, path2, frameToCompare);
    YUV1 = fopen(address2, "rb");
    while(frameToCompare < rangeStart + 1)
    {
        sprintf(address2, path2, frameToCompare);
        YUV1 = fopen(address2, "rb");
        fseek(YUV1, 0, SEEK_END);
        filelen1 = ftell(YUV1);
        rewind(YUV1);
        buffer1 = (unsigned char *)malloc(filelen1 * sizeof(unsigned char));
        int bytes_read1 = fread(buffer1, filelen1,1, YUV1);
        diffSum = 0;
        i = 0;
        while (i < filelen)
        {
            diffSum = diffSum + (((int)buffer[i]) - buffer1[i]) * (((int)buffer[i]) - buffer1[i]);
            i++;
        }
        if(diffSum < diffSumComp)
        {
            diffSumComp = diffSum;
            MostSimilarFrame = frameToCompare;
        }

        fclose(YUV1);
        free(buffer1);
        frameToCompare = frameToCompare + 1;
    }

    printf("\"MostSimilarFrame: %d\"\n", MostSimilarFrame);
    fclose(YUV);
    free(buffer);
    return MostSimilarFrame;
}


int main()
{
    FILE* excel = fopen("C:/.../Excel.csv", "a");

    int baseFrameNumber = reconstructed_frame_number;
    int rangeStart = FirstFrame(baseFrameNumber, original_frame_number);

    buffer;
    FILE *YUV1;
    frameToCompare = rangeStart;
    int temp;
    sprintf(address2, path2, frameToCompare);
    YUV1 = fopen(address2, "rb");
    int i = 0;
    double P = 0;
    double avg = 0;
    int iteration = 0;

    while(1)
    {
        MostSimilarFrame = 0;
        frameToCompare = rangeStart;
        sprintf(address1, path1, baseFrameNumber);
        FILE *YUV = fopen(address1, "rb");
        fseek(YUV, 0, SEEK_END);
        filelen = ftell(YUV);
        rewind(YUV);
        buffer = (unsigned char *)malloc(filelen * sizeof(char));
        bytes_read = fread(buffer, filelen,1, YUV);
        printf("\"baseFrameNumber: %d\"\n", baseFrameNumber);
        diffSumComp = 18446744073709551615;

        while(frameToCompare < rangeStart + alignment_range)
        {
            sprintf(address2, path2, frameToCompare);
            YUV1 = fopen(address2, "rb");
            fseek(YUV1, 0, SEEK_END);
            filelen1 = ftell(YUV1);
            rewind(YUV1);
            buffer1 = (unsigned char *)malloc(filelen1 * sizeof(unsigned char));
            bytes_read1 = fread(buffer1, filelen1,1, YUV1);
            diffSum = 0;
            i = 0;
            while (i < filelen)
            {
                diffSum = diffSum + (((int)buffer[i]) - buffer1[i]) * (((int)buffer[i]) - buffer1[i]);
                i++;
            }
            if(diffSum < diffSumComp)
            {
                diffSumComp = diffSum;
                MostSimilarFrame = frameToCompare;
            }

            fclose(YUV1);
            free(buffer1);
            frameToCompare = frameToCompare + 1;
        }

        printf("\"MostSimilarFrame: %d\"\n", MostSimilarFrame);
        fclose(YUV);
        free(buffer);
        P = PSNR(MostSimilarFrame, baseFrameNumber);
        if(P < 0)
        {
            break;
        }
        else
        {
        avg += P;
        iteration++;
        }
        printf("\"PSNR: %f\"\n", P);
        printf("--------------------------------\n");
        baseFrameNumber++;
        rangeStart++;
    }

    avg = avg / iteration;
    char s_avg[10];
    sprintf(s_avg,"%f\n", avg);
    char s_excel[10];
    sprintf(s_excel, s_avg);
    printf("\"stringExcel: %s", s_excel);
    fprintf(excel, s_excel);
    fclose(excel);

    return 0;
}

double PSNR(int frameToCompareP2, int frameToCompareP1)
{
    float pixelCount = 1.5*(width * height);
    sprintf(address1, path1, frameToCompareP1);
    FILE *YUV = fopen(address1, "rb");
    if(YUV == NULL){
        printf("\"frameToCompareP1: %d\"\n", frameToCompareP1);
        return -1;
    }

    fseek(YUV, 0, SEEK_END);
    filelen = ftell(YUV);
    rewind(YUV);
    buffer = (unsigned char *)malloc(filelen * sizeof(unsigned char));
    bytes_read = fread(buffer, filelen,1, YUV);
    fclose(YUV);

    frameToCompare = frameToCompareP2;
    sprintf(address2, path2, frameToCompare);
    FILE *YUV1;
    YUV1 = fopen(address2, "rb");
    if(YUV1 == NULL){
        printf("\"frameToCompareP2: %d\"\n", frameToCompareP2);
        return -1;
    }

    fseek(YUV1, 0, SEEK_END);
    filelen1 = ftell(YUV1);
    rewind(YUV1);
    buffer1 = (unsigned char *)malloc(filelen1 * sizeof(unsigned char));
    bytes_read1 = fread(buffer1, filelen1,1, YUV1);
    diffSum = 0;
    int i = 0;
    int count = 0;
    while (i < filelen)
    {
        diffSum = diffSum + (((int)buffer[i]) - buffer1[i]) * (((int)buffer[i]) - buffer1[i]);
        i++;
    }
    fclose(YUV1);
    free(buffer1);
    free(buffer);

    double  Max = 255 * 255;
    double  PSNR = 0;
    double  MSE = diffSum / (pixelCount);
    double logInput;
    printf("\"MSE: %f\"\n", MSE);
    if(MSE != 0)
    {
        logInput = Max/MSE;
        PSNR = 10*log10(logInput);
    }
    else{
        PSNR = 0;
    }

    return PSNR;
}
