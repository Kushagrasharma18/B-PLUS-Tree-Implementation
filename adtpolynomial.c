#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int max(int a,int b)
{
    if(a>=b)
    return a;
    else
    return b;
}
struct ppoly
{
    int coeffarr[99];
    int highpower;
} ;hgnjgjg
typedef struct ppoly *polynomial;
void zeropolynimial(polynomial poly)
{
    int i;
    for(int i=0;i<100;i++)
    {
        poly->coeffarr[i]=0;
        poly->highpower=0;
    }
}
void addpolynomial(polynomial poly1,polynomial poly2,polynomial polysum)
{
    zeropolynimial(polysum);
    polysum->highpower=max(poly1->highpower,poly2->highpower);
    printf("%d ",polysum->highpower);

    for(int i=polysum->highpower;i>=0;i--)
    {
        polysum->coeffarr[i]=poly1->coeffarr[i]+poly2->coeffarr[i];
    }
    for(int i=0;i>=polysum->highpower;i++)
    {
        printf("%d ",polysum->coeffarr[i]);
    }
}
void productpolynomial(polynomial poly1,polynomial poly2,polynomial polyprouct)
{
    zeropolynimial(polyprouct);
    polyprouct->highpower=poly1->highpower+poly2->highpower;
    for(int i=0;i<=polyprouct->highpower;i++)
    {
        for(int j=0;j<100;j++)
        {
            polyprouct->coeffarr[i]+=(poly1->coeffarr[j])*(poly2->coeffarr[99-j]);
        }

    }
}
int main()
{
    polynomial p1,p2,p3,p4;
    p1->highpower=5;
    p2->highpower=6;
    for(int i=0;i<=5;i++)
    {
        p1->coeffarr[i]=i+5;
    }
        for(int i=0;i<=6;i++)
    {
        p2->coeffarr[i]=i+8;
    }
    addpolynomial(p1,p2,p3);
    // productpolynomial(p1,p2,p4);
    printf("%d ",p3->highpower);

}
