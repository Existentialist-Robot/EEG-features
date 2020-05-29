#include "welch.h"


void comp_add(complex_t* result, const complex_t* c1, const complex_t* c2)
{
    result->r = c1->r + c2->r;
    result->i = c1->i + c2->i;
}


void comp_multiply(complex_t* result, const complex_t* c1, const complex_t* c2)
{
    result->r = c1->r * c2->r - c1->i * c2->i;
    result->i = c1->r * c2->i + c2->r * c1->i;
}


/*
 * Function:    shuffle
 * Description: �ƶ�f�д�beginPos��endPosλ�õ�Ԫ�أ�ʹ֮��λ����ż
 *              �������С�����˵��:��������f��beginPos=2, endPos=5
 *              ��shuffle���������н��Ϊf[2..5]�������У����к����
 *              λ�ö�Ӧ��ԭf��Ԫ��Ϊ: f[2],f[4],f[3],f[5]
 * Parameters:  fΪ�����������׵�ַ
 *              beginPos, endPosΪ�������±귶Χ
 */
void shuffle(complex_t* f, int beginPos, int endPos)
{
    int i;
    complex_t temp[MAX_N];

    for(i = beginPos; i <= endPos; i ++)
    {
        temp[i] = f[i];
    }

    int j = beginPos;
    for(i = beginPos; i <= endPos; i +=2)
    {
        f[j] = temp[i];
        j++;
    }
    for(i = beginPos +1; i <= endPos; i += 2)
    {
        f[j] = temp[i];
        j++;
    }
}


/*
 * Function:		evaluate
 * Description:	�Ը�������f����FFT����IFFT(��x����)���������Ϊy��
 * 			����leftPos �� rightPos֮��Ľ��Ԫ��
 * Parameters:	f : ԭʼ���������׵�ַ
 * 			beginPos : ԭʼ����������f�еĵ�һ���±�
 * 			endPos : ԭʼ����������f�е����һ���±�
 * 			x : ��ŵ�λ�������飬��Ԫ��Ϊw,w^2,w^3...
 * 			y : �������
 * 			leftPos : ��������������y��Ƭ�ϵ���ʼ�±�
 * 			rightPos : ��������������y��Ƭ�ϵ���ֹ�±�
* 			totalLength : y�ĳ���
 */
void evaluate(complex_t* f, int beginPos, int endPos, const complex_t* x, complex_t* y, int leftPos, int rightPos, int totalLength)
{
	int i;
	if ((beginPos > endPos)||(leftPos > rightPos))
	{
		printf("Error in use Polynomial!\n");
		exit(-1);
	}
	else
		if(beginPos == endPos)
		{			
			for(i = leftPos; i <= rightPos; i ++)
			{
				y[i] = f[beginPos];
			}
		}		
		else 
			if(beginPos + 1 == endPos)
			{
				for(i = leftPos; i <= rightPos; i ++)
				{
					complex_t temp;
					//comp_multiply(&temp, &f[endPos], &x[i]);
					comp_multiply(&temp, &x[i],&f[endPos]);
					comp_add(&y[i], &f[beginPos], &temp);
					//printf(" y[%d] is  %f + %fi \n",i,y[i].r,y[i].i);
				}				
			}	
			else
			{
				complex_t tempX[MAX_N],tempY1[MAX_N], tempY2[MAX_N];
				int midPos = (beginPos + endPos)/2;

				shuffle(f, beginPos, endPos);

				for(i = leftPos; i <= rightPos; i ++)
				{		
					comp_multiply(&tempX[i], &x[i], &x[i]);
				}							

				evaluate(f, beginPos, midPos, tempX, tempY1, leftPos, rightPos, totalLength);
				evaluate(f, midPos+1, endPos, tempX, tempY2, leftPos, rightPos, totalLength);

				for(i = leftPos; i <= rightPos; i ++)
				{
					complex_t temp;
					comp_multiply(&temp, &tempY2[i],&x[i]);
					comp_add(&y[i], &tempY1[i], &temp);
				}			
			}	
}


/*
 * Function:    print
 * Description: ��ӡ����Ԫ�ص�ʵ��
 * Parameters:  fΪ����ӡ������׵�ַ
 *              fLengthΪ����ĳ���
 */
void print(const complex_t* f, int fLength)
{
    BOOL isPrint = FALSE;
    int i;

    /* f[0] */
    if (abs(f[0].r) > EPS)
    {
        printf("%f", f[0].r);
        isPrint = TRUE;
    }

    for(i = 1; i < fLength; i ++)
    {
        if (f[i].r > EPS)
        {
            if (isPrint)
                printf(" + ");
            else
                isPrint = TRUE;
            printf("%ft^%d", f[i].r, i);
        }
        else if (f[i].r < - EPS)
        {
            if(isPrint)
                printf(" - ");
            else
                isPrint = TRUE;
            printf("%ft^%d", -f[i].r, i);
        }
    }
    if (isPrint == FALSE)
        printf("0");
    printf("\n");
}


/*
 * Function:    myprint
 * Description: ������ӡ��������Ԫ�أ�����ʵ�����鲿
 * Parameters:  fΪ����ӡ������׵�ַ
 *              fLengthΪ����ĳ���
 */
void myprint(const complex_t* f, int fLength)
{
    int i;
    for(i=0;i<fLength;i++)
    {
        printf("%f+%fi , ", f[i].r, f[i].i);
    }
    printf("\n");
}


/*
 * Function:   addTransTime
 * Description:�ۼƷ����������ķѵ�ʱ��
 * Parameters: toAddΪ�ۼӵ�ʱ��
 */
void addTransTime(double toAdd)
{
    transTime += toAdd;
}

