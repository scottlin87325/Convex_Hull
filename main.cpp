#include <stdio.h> 
#include <stdlib.h> 
#include <mem.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <time.h>

//定義點的結構 
typedef struct point_ {
	int x;
	int y;
}point;

//結果array
point ans[10000];

//目前ans[]的idx 
int count = 0;  

//設置座標(0,0)為視窗中心 
//最多可有800*800 = 64000點 (不重複的情況) 
int PX(int x) {
   return x+450;
}

int PY(int y) {
   return 450-y;
}

//將點繪製在視窗上 
void putpoint(point a[], int n){
	int i;
	
	for(i = 0; i < n; i++){
		setcolor(RED);
		circle(PX(a[i].x), PY(a[i].y), 2); 
		setfillstyle(SOLID_FILL, RED);
		floodfill(PX(a[i].x), PY(a[i].y), RED); 
	}
}

//坐標軸 
void axis(){
	setcolor(GREEN);
	line(0,450,900,450);
	line(450,0,450,900);
}

//亂數座標 
point* random_point(point *a, int n) { 
	srand(time(NULL));
	int i; 
	for(i = 0; i < n; i++){
		a[i].x = rand() % 801 - 400;
		a[i].y = rand() % 801 - 400;
	}
    return a;
}

//算外積判斷順逆                                           
//(原點,點1,點2) => p1 = 原點  p2 = 點1  p = 點2             
//(x, y)  (x1, y1)  (x2, y2)                                             
//向量a X 向量b = x1 * y2 - y1 * x2                            

//外積大於 0 時，兩向量前後順序為逆時針順序（在 180度 之內)
//等於 0 時，兩向量平行，也就是指夾角等於 0度 或 180度 
//小於 0 時，兩向量前後順序為順時針順序（在 180度 之內）
int findSide(point p1, point p2, point p) {
    int val = (p.y - p1.y) * (p2.x - p1.x) -   // y2 * x1 -
              (p2.y - p1.y) * (p.x - p1.x);    // y1 * x2 
  
    if (val > 0) 
        return 1;   //逆 
    if (val < 0) 
        return -1;  //順 
    return 0; 
} 
  
//算外積絕對值 
int lineDist(point p1, point p2, point p) {
    return abs ((p.y - p1.y) * (p2.x - p1.x) - 
               (p2.y - p1.y) * (p.x - p1.x)); 
} 
  
//演算法 
//(點的array, 點的個數, 點1, 點2, 方向) 
void quickHull(point a[], int n, point p1, point p2, int side) { 
    int ind = -1, i; 
    int max_dist = 0; 
    
    //找點p 距離最遠的點 
    for (i = 0; i < n; i++) { 
        int temp = lineDist(p1, p2, a[i]);   //算外積 
        if (findSide(p1, p2, a[i]) == side && temp > max_dist) {   //if方向相同 && temp > max 
            ind = i;   //ind = 那個點 
            max_dist = temp;   //max換人 
        } 
    } 
  
    //沒找到最遠點，就把L的兩端點加入結果的array + 畫線 
    if (ind == -1) { 
		int i, flag = 0, a = 0, b = 0; 
		
		//畫線 
		setcolor(WHITE);
        line(PX(p1.x),PY(p1.y),PX(p2.x),PY(p2.y));
        
        //存凸包頂點，並篩掉重複的 
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
  
    //如果找到最遠點， 遞迴運算 
    //點 a[ind]與p1連線，找最遠點 
    quickHull(a, n, a[ind], p1, -findSide(a[ind], p1, p2)); 
    //點 a[ind]與p2連線，找最遠點 
    quickHull(a, n, a[ind], p2, -findSide(a[ind], p2, p1)); 
} 
  
//印凸包 
void printHull(point a[], int n) {
	if(n < 3){
		printf("Convex Hull impossible\n\n");
		putpoint(a, n);   //印出所有點  
		return;
	}
	
    //找最左最右   
    int min_x = 0, max_x = 0, i; 
    for (i = 0; i < n; i++) {       
        if (a[i].x < a[min_x].x) 
            min_x = i; 
        if (a[i].x > a[max_x].x) 
            max_x = i; 
    }  
    
    putpoint(a, n);   //印出所有點 
    quickHull(a, n, a[min_x], a[max_x], 1);   //先算逆時針  上包 
    quickHull(a, n, a[min_x], a[max_x], -1);  //再算順時針  下包 
    
    printf("The points in Convex Hull are:\n"); 
    
    //凸包頂點高亮顯示 + 印出頂點座標 
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
	
