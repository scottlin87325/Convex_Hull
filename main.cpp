#include <stdio.h> 
#include <stdlib.h> 
#include <mem.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <time.h>

//�w�q�I�����c 
typedef struct point_ {
	int x;
	int y;
}point;

//���Garray
point ans[10000];

//�ثeans[]��idx 
int count = 0;  

//�]�m�y��(0,0)���������� 
//�̦h�i��800*800 = 64000�I (�����ƪ����p) 
int PX(int x) {
   return x+450;
}

int PY(int y) {
   return 450-y;
}

//�N�Iø�s�b�����W 
void putpoint(point a[], int n){
	int i;
	
	for(i = 0; i < n; i++){
		setcolor(RED);
		circle(PX(a[i].x), PY(a[i].y), 2); 
		setfillstyle(SOLID_FILL, RED);
		floodfill(PX(a[i].x), PY(a[i].y), RED); 
	}
}

//���жb 
void axis(){
	setcolor(GREEN);
	line(0,450,900,450);
	line(450,0,450,900);
}

//�üƮy�� 
point* random_point(point *a, int n) { 
	srand(time(NULL));
	int i; 
	for(i = 0; i < n; i++){
		a[i].x = rand() % 801 - 400;
		a[i].y = rand() % 801 - 400;
	}
    return a;
}

//��~�n�P�_���f                                           
//(���I,�I1,�I2) => p1 = ���I  p2 = �I1  p = �I2             
//(x, y)  (x1, y1)  (x2, y2)                                             
//�V�qa X �V�qb = x1 * y2 - y1 * x2                            

//�~�n�j�� 0 �ɡA��V�q�e�ᶶ�Ǭ��f�ɰw���ǡ]�b 180�� ����)
//���� 0 �ɡA��V�q����A�]�N�O���������� 0�� �� 180�� 
//�p�� 0 �ɡA��V�q�e�ᶶ�Ǭ����ɰw���ǡ]�b 180�� �����^
int findSide(point p1, point p2, point p) {
    int val = (p.y - p1.y) * (p2.x - p1.x) -   // y2 * x1 -
              (p2.y - p1.y) * (p.x - p1.x);    // y1 * x2 
  
    if (val > 0) 
        return 1;   //�f 
    if (val < 0) 
        return -1;  //�� 
    return 0; 
} 
  
//��~�n����� 
int lineDist(point p1, point p2, point p) {
    return abs ((p.y - p1.y) * (p2.x - p1.x) - 
               (p2.y - p1.y) * (p.x - p1.x)); 
} 
  
//�t��k 
//(�I��array, �I���Ӽ�, �I1, �I2, ��V) 
void quickHull(point a[], int n, point p1, point p2, int side) { 
    int ind = -1, i; 
    int max_dist = 0; 
    
    //���Ip �Z���̻����I 
    for (i = 0; i < n; i++) { 
        int temp = lineDist(p1, p2, a[i]);   //��~�n 
        if (findSide(p1, p2, a[i]) == side && temp > max_dist) {   //if��V�ۦP && temp > max 
            ind = i;   //ind = �����I 
            max_dist = temp;   //max���H 
        } 
    } 
  
    //�S���̻��I�A�N��L������I�[�J���G��array + �e�u 
    if (ind == -1) { 
		int i, flag = 0, a = 0, b = 0; 
		
		//�e�u 
		setcolor(WHITE);
        line(PX(p1.x),PY(p1.y),PX(p2.x),PY(p2.y));
        
        //�s�Y�]���I�A�ÿz�����ƪ� 
    	for(i = 0; i < count; i++) {
    		if(memcmp(&p1, &ans[i], sizeof(point)) == 0){ 
    			flag++;
    			a = 1; 
			}else if(memcmp(&p2, &ans[i], sizeof(point)) == 0){
				flag++;
				b = 1;
			}
		}
		
		if(flag == 0){
			ans[count] = p1;
        	ans[count+1] = p2;
        	count = count + 2;
        	return;
		}else if(flag == 1){
			if(a == 1){
				ans[count] = p2;
				count++;
				return;
			}else if(b == 1){
				ans[count] = p1;
				count++;
				return;
			}
		}else{
			return;
		}
    } 
  
    //�p�G���̻��I�A ���j�B�� 
    //�I a[ind]�Pp1�s�u�A��̻��I 
    quickHull(a, n, a[ind], p1, -findSide(a[ind], p1, p2)); 
    //�I a[ind]�Pp2�s�u�A��̻��I 
    quickHull(a, n, a[ind], p2, -findSide(a[ind], p2, p1)); 
} 
  
//�L�Y�] 
void printHull(point a[], int n) {
	if(n < 3){
		printf("Convex Hull impossible\n\n");
		putpoint(a, n);   //�L�X�Ҧ��I  
		return;
	}
	
    //��̥��̥k   
    int min_x = 0, max_x = 0, i; 
    for (i = 0; i < n; i++) {       
        if (a[i].x < a[min_x].x) 
            min_x = i; 
        if (a[i].x > a[max_x].x) 
            max_x = i; 
    }  
    
    putpoint(a, n);   //�L�X�Ҧ��I 
    quickHull(a, n, a[min_x], a[max_x], 1);   //����f�ɰw  �W�] 
    quickHull(a, n, a[min_x], a[max_x], -1);  //�A�ⶶ�ɰw  �U�] 
    
    printf("The points in Convex Hull are:\n"); 
    
    //�Y�]���I���G��� + �L�X���I�y�� 
    for(i = 0; i < count; i++){
    	printf("( %d , %d ) ", ans[i].x, ans[i].y);
    	setcolor(YELLOW);
    	circle(PX(ans[i].x), PY(ans[i].y), 5); 
		setfillstyle(SOLID_FILL, YELLOW);
		floodfill(PX(ans[i].x), PY(ans[i].y), YELLOW); 
	}
	printf("\n\n");
	count = 0;
} 
  
int main() { 
	initwindow(900,900);
	while(1){
		int n;
	    axis();
	    
	    printf("Number of points: ");
	    scanf("%d", &n);
	    point a[n];
	    
		random_point(a, n);
	    printHull(a, n); 
	    getch();
   		cleardevice(); 
	}
	return 0;
} 
	
