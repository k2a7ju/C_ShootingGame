
/*****
g1444666
Hiroyuki Shirakazu
最終課題
*****/


#include<stdio.h>
#include<stdlib.h>
#include<eggx.h>
#include<time.h>
#include<math.h>

#define M_PI 3.14159265358979323846264338327950288
#define Ra 17
//ライバルの頭の半径
#define ra 5
//Playerの玉の半径
#define Ra2 20
//Playerの頭の半径
#define ra2 9
#define RIVALHP 4
#define PLAYERHP 3
#define MOVEDOT 10
#define MAXBALL 10


int start(void);
void talkwin(void);
int timer(int early_time);
int collision(int x1, int y1, int x2, int y2,int syahen);
void globe(double globe_x, double globe_y);
void bird(double x, double y, double size,int wing);
void rival_ball(double rivalBall_x,double rivalBall_y);
void rival(double rival_x, double rival_y);
void cloud();
void draw_FlyingBoy(double x,double y);
void heart(int color, int x,int y);
void HP_item(double x, double y);
void explosion(int x, int y);
void death(int x, int y);
void won(void);
void loss(void);
void Layer(void);

int win;
int player_hp = PLAYERHP;//プレイヤーのHP
int rival_hp = RIVALHP;//敵のHP
int endtime = 0;
int layerflag = 0;


int main(){
	
	int i;	//カウンタ変数;
	int mode = 0;	//モード選択
	
	int boy_x = 250, boy_y = 150;	//空飛ぶ少年の初期座標
	int bird_x = 510, bird_y;	//鳥の座標
	int size = 10; //鳥の大きさ
	int bird_count = 0;	//鳥の描写に使用
	
	double rival_x = 250, rival_y = 400;	//敵の座標
	double globe_x, globe_y;	//Playerの出す球
	int circle_x[MAXBALL],circle_y[MAXBALL];	//Playerの玉の座標
	double rivalBall_x = 0, rivalBall_y = 0;//敵の玉の座標
	double rivalBall2_x = 0, rivalBall2_y = 0;	//敵の玉の座標
	int item_x = 0, item_y = 0;	//アイテムの座標
	
	int heart_x = 220, heart_x2 = 380, heart_y = 30;	//ハートの座標
	
	int early_time;	//開始時間
	int now = 0;	//今の時間
	int ex_r_time, ex_p_time;	//爆発する時間を管理する
	int rival_flag = 0;
	
	int flag = 0, shotflag[MAXBALL];	//Playerの玉を描写する時に使う
	int item_flag = 0;	//アイテムを描写する時に使用するフラグ
	int which_item_flag = 0;	//どのアイテムが出るかを管理するフラグ;
	int hit_flag2 = 0, hit_flag3 = 0;
	int rivalBall_flag = 0, rivalBall2_flag = 0;
	int ex_flag = 0, ex_flag2 = 0, ex_flag3 = 0;	//爆発を描写させるかどうかを管理するフラグ
	int bird_flag = 0;	//鳥の描写に使うフラグ
	int cd_flag1 = 0;	//playerの玉が敵に当たった時のフラグ
	int cd_flag2 = 0;	//難しいモードで敵の移動につかうフラグ
	int cd_flag3 = 0;	//敵の右手の玉がplayerに当たった時のフラグ
	int cd_flag4 = 0;	//敵の左手の玉がplayerに当たった時のフラグ
	int cd_flag5 = 0;	//playerの玉が鳥に当たった時のフラグ
	int item_getflag;	//アイテムを取った時のフラグ
	int judge = 1;	//難しいモードで使用するフラグ
	
	
	double rad = -M_PI;	//ラジアン
	int key_code;	//キー入力の値
	int rival_color;	//敵の色を決める
	
	int dx,dy;	//難しいモードで使用
	int t_x = 0, t_y = 0;	//難しいモードで使用
	
	win = gopen(500,500);
	winname(win,"Game");
	srand(time(NULL));
	
	mode = start();
	
	/* 配列の初期化 */
	for(i=0;i<MAXBALL;i++){
		circle_x[i] = 0;
		circle_y[i] = 0;
		shotflag[i] = 0;
	}
	
	
	/* 開始時間の保存 */
	early_time = time(NULL);
	
	/* ノンブロッキングモード開始 */
	gsetnonblock(ENABLE);
	
	layer(win,0,1);
	while(1){
		
		/* 背景の設定 */
		gsetbgcolor(win,"RoyalBlue");
		gclr(win);
		
		
		cloud();
		
		
		
		/* 鳥の描写 */
		if((now%10 == 0) && (bird_flag == 0)){
			bird_y = rand()%200 + 280;
			bird_flag = 1;
		}
		if(bird_flag != 0){
			bird(bird_x,bird_y,size,bird_count);
			bird_x -= 3;
			
			if(bird_x <= -5){
				bird_flag = 0;
				bird_x = 510;
			}
			bird_count++;
			
			if(bird_count%8 == 0){
				bird_count = 0;
			}
		}
		
		
		
		/* 	敵の移動の処理部分	*/
		/* 簡単モードの時 */
		if(mode == 1){
			
			rival_x=rad/M_PI*100;
			rival_y=60*sin(rad) + 400;
			
			if(rival_flag == 0 && rival_x >= 520){
				rival_flag = 1;
			}
			else if(rival_x <= -20){
				rival_flag = 0;
			}
			if(rival_flag != 0){
				rad -= 0.2;
			}
			else{
				rad += 0.2;
			}
		}
		
		
		
		/* 難しいモードの時 */
		else if(mode == 2){
			
			cd_flag2 = collision(t_x,t_y,rival_x,rival_y,MOVEDOT+MOVEDOT);
			
			if(cd_flag2 == 1){
				cd_flag2 = 0;
				judge = 1;
			}
			
			if(judge == 1){
				
				while(1){
					t_x = rand()%450 + 20;
					t_y = rand()%200 + 290;
					if(200 <=  t_y && t_y <= 450){
						judge = 0;
						break;
					}
				}
				dx = (t_x - rival_x)/12;
				dy = (t_y - rival_y)/12;
			}
			
			if( 10<= rival_x && rival_x <= 490 && 10 <= rival_y && rival_y <= 490){
				rival_x += dx;
				rival_y += dy;
			}
			
		}
		else{
			rival_x = 250;
			rival_y = 300;
		}
		
		
		
		rival(rival_x, rival_y);
		draw_FlyingBoy(boy_x,boy_y);
		
		
		
		/* 敵の球の表示 */
		/* 左手側 */
		if((now >= 3) && (now%2 == 0)){
			rivalBall_x = rival_x;
			rivalBall_y = rival_y;
			rivalBall_flag = 1;
			hit_flag2 = 0;
		}
		
		if(rivalBall_flag != 0){
			
			rival_ball(rivalBall_x-24, rivalBall_y-25);
			
			if(rivalBall_y >= 0){
				rivalBall_y -= 15;
			}
		}
		/* 右手側 */
		if(now%3 == 0){
			rivalBall2_x = rival_x;
			rivalBall2_y = rival_y;
			rivalBall2_flag = 1;
			hit_flag3 = 0;
			
		}
		if(rivalBall2_flag != 0){
			rival_ball(rivalBall2_x+24, rivalBall2_y-25);
			
			if(rivalBall2_y >= 0){
				rivalBall2_y -= 15;
			}
		}
		
		
		
		/* Playerの球の表示 */
		if(flag != 0){
			
			for(i=0;i<MAXBALL;i++){//使っていない配列を探す
				
				if(shotflag[i] == 0){
					flag = 0;
					circle_x[i] = boy_x;
					circle_y[i] = boy_y;
					shotflag[i] = 1;
					break;
				}
			}
			
		}
		for(i=0;i<MAXBALL;i++){//描写
			
			if(shotflag[i] != 0){
				globe(circle_x[i],circle_y[i]+25);
			}
			
			if(circle_y[i] >= 500){//500超えるまで描写
				shotflag[i] = 0;
			}
		}
		
		for(i=0;i<MAXBALL;i++){//描写中の玉が進むようにする
			
			if(shotflag[i] != 0){
				circle_y[i] += 10;
			}
		}
		
		
		
		/* Playerの移動 */
		key_code = ggetch();
		
		if(boy_x <=470 && key_code == 28){
			boy_x += 15;
		}
		else if(boy_x>=30 && key_code == 29){
			boy_x -= 15;
		}
		else if(key_code == 30){
			boy_y += 15;
		}
		else if(boy_y>=90 && key_code == 31){
			boy_y -= 15;
		}
		
		if(key_code == 32){
			globe_x = boy_x;
			globe_y = boy_y + 22;
			flag = 1;
		}
		
		
		
		/* ゲージ等の表示 */
		/* 時間の表示 */
		newpen(win,1);
		now = timer(early_time);	//経過時間
		drawstr(win, 15.0, 20.0, 16, 1.0, "Time : %d s",now);
		/* PlayerのHPを表示 */
		drawstr(win, 120.0, 20.0, 16, 1.0, "Player HP : ");
		rival_color = 0;
		
		for(i=0;i<player_hp;i++){
			heart(rival_color,heart_x,heart_y);
			heart_x += 18;
			
			if(i == player_hp-1){
				heart_x = 220;
			}
		}
		/* RivalのHPを表示 */
		newpen(win,1);
		drawstr(win, 290.0, 20.0, 16, 1.0, "Rival HP : ");
		rival_color = 1;
		
		for(i=0;i<rival_hp;i++){
			heart(rival_color,heart_x2,heart_y);
			heart_x2 += 18;
			
			if(i == rival_hp-1){
				heart_x2 = 380;
			}
		}
		
		
		
		/* 当たり判定 */
		/* playerの攻撃が敵に当たった時 */
		for(i=0;i<MAXBALL;i++){
			
			if(shotflag[i] != 0){
				cd_flag1 = collision(circle_x[i],circle_y[i],rival_x,rival_y,Ra+ra);
				
				if(cd_flag1 == 1){
					cd_flag1 = 0;
					ex_r_time = timer(early_time);
					rival_hp -= 1;
					ex_flag = 1;
					shotflag[i] = 0;
					
					if(rival_hp == 0){
						endtime = now;
						break;	//forを抜ける
						
					}//if
				}//if
			}//if
		}//for
		if(rival_hp == 0){
			break;	//whileを抜ける
		}
		
		if(ex_flag != 0){
			
			if(now ==  ex_r_time+ 1){
				ex_flag = 0;
			}
		}
		/* 敵の 右 攻撃がplayerに当たった時 */
		cd_flag3 = collision(boy_x,boy_y,rivalBall2_x+24,rivalBall2_y-25,Ra2+ra2);
		
		if((cd_flag3 == 1) && (hit_flag3 == 0)){
			cd_flag3 = 0;
			player_hp -= 1;
			hit_flag3 = 1;
			ex_p_time = timer(early_time);
			ex_flag3 = 1;
			
			if(player_hp == 0){
				endtime = now;
				break;
			}
			
		}
		/* 敵の 左 攻撃がplayerに当たった時 */
		cd_flag4 = collision(boy_x,boy_y,rivalBall_x-24,rivalBall_y-25,Ra2+ra2);
		
		if((cd_flag4 == 1) && (hit_flag2 == 0)){
			cd_flag4 = 0;
			player_hp -= 1;
			ex_p_time = timer(early_time);
			hit_flag2 = 1;
			ex_flag2 = 1;
			
			if(player_hp == 0){
				endtime = now;
				break;
			}
		}
		/* playerの攻撃が 鳥 に当たった時 */
		for(i=0;i<MAXBALL;i++){
			
			if(shotflag[i] != 0){
				cd_flag5 = collision(circle_x[i],circle_y[i],bird_x,bird_y,size*2);
				
				if(cd_flag5 == 1){
					cd_flag5 = 0;
					item_x = bird_x - 5;
					item_y = bird_y - 5;
					bird_x = 510;	//初期値に戻す
					item_flag = 1;
					bird_flag = 0;
				}
			}
		}
		
		
		
		/* アイテムを描写 */
		if(item_flag != 0){	//アイテム描写のフラグが立った時
			
			if(rand()%2 == 0){
				which_item_flag = 1;
			}
			
			
			if(which_item_flag == 1){	//回復アイテム
				HP_item(item_x,item_y);
			}
			
			item_y -= 5;
			if(item_y <= -20){
				item_flag = 0;
				which_item_flag = 0;
			}
			
			/* アイテムとplayerの当たり判定 */
			item_getflag = collision(item_x,item_y,boy_x+7,boy_y+3,Ra2+10);
			
			if(item_getflag == 1){
				item_getflag = 0;
				item_flag = 0;
				which_item_flag = 0;
				
				if(player_hp < PLAYERHP){
					player_hp++;
				}
			}
		}
		
		
		
		/* プレイヤーの爆発している時間を管理 */
		if(ex_flag2 != 0){
			
			if(now == ex_p_time + 1){
				ex_flag2 = 0;
			}
			
		}
		else if(ex_flag3 != 0){
			
			if(now == ex_p_time + 1){
				ex_flag3 = 0;
			}
		}
		
		
		
		/* デバッグ */
		if(key_code == 48){
			player_hp -= 1;
			hit_flag2 = 1;
			ex_flag2 = 1;
			
			if(player_hp == 0){
				endtime = now;
				break;
			}
		}
		
		
		
		/* 敵の爆発の描写 */
		if(ex_flag != 0){
			explosion(rival_x,rival_y-30);
		}
		/* Playerの爆発の描写 */
		if(ex_flag2 != 0){//左
			explosion(boy_x,boy_y);
		}
		else if(ex_flag3 != 0){//右
			explosion(boy_x,boy_y);
		}
		
		Layer();
		
		msleep(20);
	}//while(1)
	
	
	/* 死亡 */
	if(rival_hp == 0){
		death(rival_x,rival_y);
	}
	else{
		death(boy_x,boy_y);
	}
	
	if(rival_hp == 0){
		won();
	}
	else{
		loss();
	}
	ggetch();
	exit(0);
}

/* スタート画面の描写 */
int start(){
	
	int i = 0;
	int key_count = 0;
	int key_code;
	int mode = 0;	 //1、簡単	2、難しい
	int flag = 0;
	gsetnonblock(ENABLE);
	
	newpen(win,1);
	
	while(1){
		
		talkwin();
		
		key_code = ggetch();
		
		if(key_code == 97){
			key_count += 1;
		}
		
		drawstr(win,30,160,FONTSET,0.0,"「フォッフォッフォッ。」");
		
		if(key_count > 0){
			drawstr(win,30,130,FONTSET,0.0,"「おぉ、来てくれたかアンパ◯マン。」");
		}
		if(key_count > 1){
			drawstr(win,30,100,FONTSET,0.0,"「君に頼みがある。悪の帝王が現れたのだ。」");
		}
		if(key_count > 2){
			drawstr(win,30,70,FONTSET,0.0,"「あいつを放っておく訳にはいかない。倒してほしいのじゃ !」");
		}
		if(key_count > 3){
			i = 0;
			key_count = 0;
			gclr(win);
			break;
		}
		
		msleep(50);
		gclr(win);
		i++;
	}//wihile()
	
	while(1){
		
		talkwin();
		key_code = ggetch();
		
		if(key_code == 97){
			key_count += 1;
		}
		
		if(key_count >= 61){
			i = 0;
			key_count = 0;
			gclr(win);
			break;
		}
		drawstr(win,30,160,FONTSET,0.0,"「よし、レベルを選ぶのじゃ。」");
		if(key_count > 0){
			if(mode == 2){
				newpen(win,0);
			}
			drawstr(win,30,130,FONTSET,0.0,"[1] かんたん。");
			if(mode == 1){
				newpen(win,0);
			}
			else{
				newpen(win,1);
			}
			drawstr(win,200,130,FONTSET,0.0,"[2] むずかしい。");
			newpen(win,1);
			
			/* モード選択 */
			if(flag == 0){
				flag = 1;
				
				while(1){
					key_code = ggetch();
					
					if(key_code == 49){
						mode = 1;
						break;
					}
					else if(key_code == 50){
						mode = 2;
						break;
					}
					else if(key_code == 51){
						mode = 3;
						break;
					}
				}
			}
		}
		
		if(mode == 1 || mode == 2  || mode == 3 ){
			drawstr(win,30,100,FONTSET,0.0,"「操作は簡単。十字キーで移動。スペースキーで口から何かが出る。」");
			drawstr(win,30,70,FONTSET,0.0,"「それで攻撃するのじゃ。」");
			key_count = 60;
		}
	}//while(1)
	
	while(1){
		
		talkwin();
		
		key_code = ggetch();
		
		if(key_code == 97){
			key_count += 1;
		}
		drawstr(win,30,160,FONTSET,0.0,"「お互いの弱点は共に顔じゃ！」");
		if(key_count > 0){
			drawstr(win,30,130,FONTSET,0.0,"「絶対に敵の攻撃には当たるのではないぞ。」");
		}
		if(key_count > 1){
			drawstr(win,30,100,FONTSET,0.0,"「ちなみに、鳥を撃ち落とすとアイテムが出る。それは覚えておけ」");
		}
		if(key_count > 2){
			drawstr(win,30,70,FONTSET,0.0,"「では、行ってくるのじゃ！！！！」");
		}
		if(key_count > 3){
			i = 0;
			key_count = 0;
			gclr(win);
			break;
		}
		msleep(50);
		gclr(win);
		i++;
	}//wihile()
	return mode;
}

/* トークウィンドウの描写 */
void talkwin(){
	
	drawrect(win,20,40,460,150);
	drawcirc(win,450,70,17,17);
	drawstr(win,447,65.0,16,1.0,"A");
	newpen(win,1);
	fillarc(win, 450,95,10,10,65,115,1);
}

/* 時間管理 */
int timer(int early_time){
	
	int late_time;
	late_time = time(NULL);
	late_time -= early_time;
	return late_time;
}

/* 当たり判定 */
int collision(int x1, int y1, int x2, int y2,int syahen){
	
	int x_line, y_line, ano_line;
	
	x_line = x2 - x1;
	x_line *= x_line;
	y_line = y2 -y1;
	y_line *= y_line;
	ano_line = x_line + y_line;
	ano_line = sqrt(ano_line);
	
	//当たり判定を検出すると1を返し、それ以外の時は0を返す
	if(syahen >= ano_line){
		return 1;
	}
	else{
		return 0;
	}
}

/* playerの玉の描写 */
void globe(double globe_x, double globe_y){
	
	newpen(win,7);
	fillcirc(win,globe_x,globe_y,ra,10);
}

/* 鳥の描写 */
void bird(double x, double y, double size,int wing){
	
	/* 奥の羽 */
	newrgbcolor(win, 0,255,255);
	if(wing == 1){
		fillarc(win,x-4,y-5,size+12,size+15,40,60,1);
	}
	else{
		fillarc(win,x-3,y-5,size+12,size+15,300,320,1);
	}
	/* 胴体 */
	newrgbcolor(win, 0,51,255);
	fillcirc(win, x, y, size, size);
	fillcirc(win, x+3, y-2, size, size);
	fillcirc(win, x+5, y-4, size, size);
	fillarc(win,x+32,y-5,size+10,size+10,160,200,1);
	fillarc(win,x+28,y-4,size,size,-35,55,1);
	/* くちばし */
	newpen(win,7);
	fillarc(win,x-15,y-1,size-3,size-3,-20,30,1);
	/* 目 */
	newpen(win,0);
	fillcirc(win, x-1, y+1, 2, 2);
	
	/* 手前の羽 */
	newrgbcolor(win, 0,255,255);
	if(wing == 1){
		fillarc(win,x+5,y-5,size+12,size+15,30,50,1);
	}
	else{
		fillarc(win,x+5,y-5,size+12,size+15,310,330,1);
	}
}

/* 敵の玉の描写 */
void rival_ball(double rivalBall_x,double rivalBall_y){
	
	newrgbcolor(win,255,0,255);
	fillcirc(win,rivalBall_x,rivalBall_y,ra2,ra2);
	newpen(win,1);
	fillcirc(win,rivalBall_x,rivalBall_y,4,4);
}

/* 敵の描写 */
void rival(double rival_x, double rival_y){
	
	int i;//カウンタ変数
	
	/* マント */
	if(rival_hp > RIVALHP-2){
		newpen(win,2);
	}
	else{
		newpen(win,0);
	}
	fillarc(win, rival_x,rival_y-10,65,65, 230,310, 1);
	/* 頭 */
	if(rival_hp > RIVALHP-2){
		newpen(win,0);
	}
	else{
		newpen(win,2);
	}
	fillcirc(win,rival_x,rival_y,Ra,23);
	/* 冠 */
	newpen(win,7);
	fillrect(win,rival_x-12,rival_y+13,25,9);
	fillarc(win,rival_x,rival_y+35,20,20,255,285,1);
	fillarc(win,rival_x-7.5,rival_y+35,20,20,255,285,1);
	fillarc(win,rival_x+7.5,rival_y+35,20,20,255,285,1);
	/* 胴体 */
	if(rival_hp > RIVALHP-2){
		newpen(win,0);
	}
	else{
		newpen(win,2);
	}
	int size = 7;
	
	for(i=7;i<=49;i+=7){
		fillcirc(win,rival_x,rival_y-i,size,size);
	}
	/* 目 */
	newpen(win,1);
	fillcirc(win,rival_x+6,rival_y,4,7);
	fillcirc(win,rival_x-6,rival_y,4,7);
	/* 右腕 */
	if(rival_hp > RIVALHP-2){
		newpen(win,0);
	}
	else{
		newpen(win,2);
	}
	size = 5;
	fillcirc(win,rival_x+5,rival_y-22,size,size);
	fillcirc(win,rival_x+10,rival_y-22,size,size);
	fillcirc(win,rival_x+15,rival_y-22,size,size);
	fillcirc(win,rival_x+20,rival_y-22,size,size);
	fillcirc(win,rival_x+23,rival_y-26,size,size);
	fillcirc(win,rival_x+26,rival_y-30,size,size);
	/* 左腕 */
	if(rival_hp > RIVALHP-2){
		newpen(win,0);
	}
	else{
		newpen(win,2);
	}
	size = 5;
	fillcirc(win,rival_x-5,rival_y-22,size,size);
	fillcirc(win,rival_x-10,rival_y-22,size,size);
	fillcirc(win,rival_x-15,rival_y-22,size,size);
	fillcirc(win,rival_x-20,rival_y-22,size,size);
	fillcirc(win,rival_x-23,rival_y-26,size,size);
	fillcirc(win,rival_x-26,rival_y-30,size,size);
	/* 右脚 */
	size = 6;
	fillcirc(win,rival_x+5,rival_y-49,size,size);
	fillcirc(win,rival_x+10,rival_y-53,size,size);
	fillcirc(win,rival_x+15,rival_y-57,size,size);
	fillcirc(win,rival_x+20,rival_y-61,size,size);
	fillcirc(win,rival_x+23,rival_y-65,size,size);
	fillcirc(win,rival_x+26,rival_y-69,size,size);
	/* 左脚 */
	size = 6;
	fillcirc(win,rival_x-5,rival_y-49,size,size);
	fillcirc(win,rival_x-10,rival_y-53,size,size);
	fillcirc(win,rival_x-15,rival_y-57,size,size);
	fillcirc(win,rival_x-20,rival_y-61,size,size);
	fillcirc(win,rival_x-23,rival_y-65,size,size);
	fillcirc(win,rival_x-26,rival_y-69,size,size);
}

/* 雲の描写 */
void cloud(){
	
	int circle_size = 15;
	int i;
	static double cloud_x[5],cloud_y[5];
	static int flag = 0;
	
	/* 雲の初期位置決定 */
	if(flag == 0){
		flag = 1;
		for(i=0;i<5;i++){
			cloud_x[i] = i*100;
			
			if(i%2 == 0){
				cloud_y[i] = 450;
			}
			else if(i%3 == 0){
				cloud_y[i] = 500;
			}
			else{
				cloud_y[i] = 370;
			} 
		}
	}
	
	/* 雲の描写 */
	for(i=0;i<5;i++){
		newpen(win,1);
		fillcirc(win,cloud_x[i],cloud_y[i], circle_size, circle_size);
		fillcirc(win,cloud_x[i]+10, cloud_y[i]+10, circle_size, circle_size);
		fillcirc(win,cloud_x[i]+20, cloud_y[i], circle_size, circle_size);
		fillcirc(win,cloud_x[i]+30, cloud_y[i]+10, circle_size, circle_size);
		fillcirc(win,cloud_x[i]+40, cloud_y[i], circle_size, circle_size);
		fillcirc(win,cloud_x[i]+50, cloud_y[i]+10, circle_size, circle_size);
	}
	
	/* 雲の移動 */
	for(i=0;i<5;i++){
		cloud_x[i] += 0.3;
	}
	
	/* 画面を出た時に雲を左に戻す */
	for(i=0;i<5;i++){
		if(cloud_x[i] >=510){
			srand(time(NULL));
			cloud_x[i] = -60;
			cloud_y[i] = (double)(rand()%100+400);
		}
	}
}

/* playerの描写 */
void draw_FlyingBoy(double x,double y){
	
	/* 右腕 */
	newpen(win,2);
	fillcirc(win,x+15,y,10,10);
	fillcirc(win,x+20,y+5,10,10);
	fillcirc(win,x+25,y+10,10,10);
	/* 右腕の拳 */
	newpen(win,7);
	fillcirc(win,x+25,y+15,9,9);
	/* 左腕 */
	newpen(win,2);
	fillcirc(win,x-15,y,10,10);
	fillcirc(win,x-20,y+5,10,10);
	/* 左腕の拳 */
	newpen(win,7);
	fillcirc(win,x-20,y+10,9,9);
	/* 左足 */
	newpen(win,2);
	fillcirc(win,x-10,y-65,8,5);
	fillcirc(win,x-10,y-70,8,5);
	fillcirc(win,x-10,y-75,8,5);
	/* 右足 */
	fillcirc(win,x+7,y-65,8,5);
	fillcirc(win,x+7,y-70,8,5);
	fillcirc(win,x+7,y-75,8,5);
	/* 右足首 */
	newpen(win,7);
	fillcirc(win,x+7,y-82,10,10);
	/* 左足首 */
	fillcirc(win,x-10,y-82,10,10);
	/* マントの影 */
	newpen(win,0);
	fillarc(win, x,y-13,50,50, 228,302, 1);
	/* マント本体 */
	newpen(win,10);
	fillarc(win, x,y+5,67,67, 230,300, 1);
	/* 頭 */
	newrgbcolor(win, 204,102,0);
	fillcirc(win,x,y,Ra2,Ra2);
}

/* ハートの描写*/
void heart(int color, int x,int y){
	
	if(color == 0){
		newrgbcolor(win, 255,0,153);
	}
	else{
		newpen(win,0);
	}
	fillcirc(win,x,y,4,4);
	fillcirc(win,x+8,y,4,4);
	fillarc(win,x+3,y-13,12,12,56,120,1);
}

/* アイテムの描写 */
void HP_item(double x, double y){
	
	newrgbcolor(win, 255,0,102);
	fillrect(win,x,y,15,20);
	fillarc(win,x+7,y+17,15,15,60,120,1);
	newpen(win,1);
	drawstr(win,x+3,y+9,7, 0.0, "HP");
	
}

/* 爆発の描写 */
void explosion(int x, int y){
	
	double r1;//半径
	for(r1=0;r1<=70;r1+=0.1){
		newpen(win,2);
		fillcirc(win,x,y,r1,r1);
		newpen(win,7);
		fillcirc(win,x,y,r1-30,r1-30);
	}
}

/* 死んだ時の画面の描写 */
void death(int x, int y){
	
	int i = 0;
	int high;
	layer(win,0,1);
	layerflag = 0;
	for(i=0;i<=400;i++){
		newpen(win,2);
		fillcirc(win,x,y,i,i);
		newpen(win,7);
		fillcirc(win,x,y,i-50,i-50);
		
		Layer();
		
		msleep(2);
	}
	
	for(i=500,high=0;i>=-1;i--,high++){
		newpen(win,0);
		fillrect(win,0,i,500,high);
		
		Layer();
		
		msleep(2);
	}
	gsetbgcolor(win,"Black");
	gclr(win);
}

/* 勝った時の画面の描写 */
void won(void){
	
	int key_count = 0;
	int key_code;
	gsetnonblock(ENABLE);
	
	newpen(win,1);
	
	while(1){
		
		talkwin();
		key_code = ggetch();
		
		drawstr(win,230,300,24,0.0,"WIN !");
		
		newpen(win,1);
		if(key_code == 97){
			key_count += 1;
		}
		drawstr(win,30,160,FONTSET,0.0,"「よくやった。アンパ◯マン!」");
		if(key_count > 0){
			drawstr(win,30,130,FONTSET,0.0,"「倒すのにかかった時間は %d秒 じゃったぞ！」",endtime);
		}
		if(key_count > 1){
			drawstr(win,30,100,FONTSET,0.0,"「また奴が現れた時は宜しく頼む！」");
		}
		if(key_count > 2){
			drawstr(win,30,70,FONTSET,0.0,"「では。」");
		}
		
		if(key_count > 3){
			key_count = 0;
			gclr(win);
			break;
		}
		
		Layer();
		
		msleep(50);
		gclr(win);
	}
}

/* 負けた時の画面 */
void loss(void){
	
	int key_count = 0;
	int key_code;
	gsetnonblock(ENABLE);
	
	newpen(win,1);
	
	while(1){
		
		talkwin();
		key_code = ggetch();
		
		drawstr(win,230,300,24,0.0,"LOSE !");
		
		newpen(win,1);
		if(key_code == 97){
			key_count += 1;
		}
		
		if(endtime < 10){
			drawstr(win,30,160,FONTSET,0.0,"「%d秒で死ぬとは何事じゃ！！！」",endtime);
		}
		else if(10 <= endtime && endtime < 20){
			drawstr(win,30,160,FONTSET,0.0,"「%d秒しか戦えぬのかお前は。」",endtime);
		}
		else if(20 <= endtime){
			drawstr(win,30,160,FONTSET,0.0,"「%d秒か。まぁまぁ頑張ったな。」",endtime);
		}
		if(key_count > 0){
			drawstr(win,30,130,FONTSET,0.0,"「また戦ってくれるのを待っておるぞ。」");
		}
		if(key_count > 1){
			drawstr(win,30,100,FONTSET,0.0,"「では。」");
		}
		if(key_count > 2){
			key_count = 0;
			gclr(win);
			break;
		}
		Layer();
		
		msleep(50);
		gclr(win);
	}
}

void Layer(){
	
	if(layerflag == 0){
		layer(win,0,1);
		layerflag =  1;
	}
	else{
		layer(win,1,0);
		layerflag =  0;
		
	}
	
}