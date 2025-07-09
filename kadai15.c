
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define imageWidth 8
#define imageHeight 8

#define XMIN 32
#define YMIN 24
#define XMAX 640
#define YMAX 480

float radius = 1.0;
float height = 0.0;  /* 高さ */
float y_max = 10.0;  /* 初期位置 */
float coeff = 0.9;   /* 弾性係数 */
float gravt = -9.81; /* 重力加速度 */
float dt = 0.1;      /* 時間の刻み */
float vz = 0.0;      /* 初速度 */
float vzt = 0.0;
float eps = 0.01;
int flagj = 0;
int hFlag = 0;
int flagm = 0;

float xx = 0., yy = 0.;
float xx0 = 0., yy0 = 0.;
float xx1 = 0., yy1 = 0.;

float param = 0.0;
float headparam = 0.0;
float epsilon = 1.0e-4;
int flagh = 0;

float z = 10.0; // camera距離

float diffuse[] = {0.8, 0.8, 0.8, 1.0};  /* 拡散反射成分の反射強度*/
float specular[] = {1.0, 1.0, 1.0, 1.0}; /* 鏡面反射成分の反射強度 */
float ambient[] = {0.2, 0.2, 0.2, 1.0};  /* 環境光成分の反射強度*/
float shininess = 128.0;
float light[] = {1.5, 1.5, 0.0, 0.0};    /* 光源の位置と種類 (平行光源)*/
float emission[] = {1.0, 1.0, 0.0, 1.0}; /* 放射光の属性 */

unsigned char image1[imageHeight][imageWidth][4];

unsigned char texture1[imageHeight][imageWidth] = /* texture1の定義 */
    {
        {255, 255, 255, 255, 0, 0, 0, 0},
        {255, 255, 255, 255, 0, 0, 0, 0},
        {255, 255, 255, 255, 0, 0, 0, 0},
        {255, 255, 255, 255, 0, 0, 0, 0},
        {0, 0, 0, 0, 255, 255, 255, 255},
        {0, 0, 0, 0, 255, 255, 255, 255},
        {0, 0, 0, 0, 255, 255, 255, 255},
        {0, 0, 0, 0, 255, 255, 255, 255}};

void makeImage1(void) /* texture1 をimage1に登録 */
{
    int i, j;
    for (i = 0; i < imageHeight; i++)
    {
        for (j = 0; j < imageWidth; j++)
        {
            image1[i][j][0] = (unsigned char)texture1[i][j];
            image1[i][j][1] = (unsigned char)texture1[i][j];
            image1[i][j][2] = (unsigned char)texture1[i][j];
            image1[i][j][3] = (unsigned char)100;
        }
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* zバッファを初期化 */

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glEnable(GL_LIGHTING);
    // 胴体を描いた後頭部の位置に移動してそこで頭部を回転。
    // 頭部の描画部分をglPushMatrix()とglPopMatrix()で囲む。

    // 胴体
    glPushMatrix();
    glTranslatef(xx + 0.0, yy - 1.85 + height, 0.0);
    // 胴体の色を青に設定
    float difblue[] = {0.0, 0.0, 1.0, 1.0};      // 青の拡散反射
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difblue); // 拡散反射成分を青色に設定

    glutSolidCube(1.5); // glutSolidCone(double radius, double height, int slices, int stacks)
    glPopMatrix();

    float dif[] = {0.8, 0.8, 0.8, 1.0};      // 元の色
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif); // 拡散反射成分を戻す

    // 頭部
    glPushMatrix();
    glTranslatef(xx + 0.0, yy - 0.2 + height, 0.0); // 頭部の基準位置に移動（胴体の上）
    glRotatef(headparam, 0.0, 1.0, 0.0);// y軸周りにheadparam度回転
    // glTranslatef(0.0, 1.0, 0.0);
    glutSolidCube(2.0);

    // 左目
    glPushMatrix();

    // glScalef(zoom, zoom, 1.0f);

    glTranslatef(-0.5, 0.5, 1.0);
    // glScalef(zoom, zoom, zoom);
    glutSolidSphere(0.2, 10, 10);

    glPopMatrix();

    // 右目
    glPushMatrix();
    glTranslatef(0.5, 0.5, 1.0);
    glutSolidSphere(0.2, 10, 10);
    glPopMatrix();

    // 鼻
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.1);
    glutSolidCube(0.2);
    glPopMatrix();

    // 右耳
    glPushMatrix();
    glTranslatef(0.8, 1.0, 0.75);
    glRotatef(-90.0, 1.0, 0.0, 0.0); // x軸方向に-90度回転
    glutSolidCone(0.2, 0.7, 20, 10); // glutSolidCone(double radius, double height, int slices, int stacks)
    glPopMatrix();

    // 左耳
    glPushMatrix();
    glTranslatef(-0.8, 1.0, 0.75);
    glRotatef(-90.0, 1.0, 0.0, 0.0); // x軸方向に-90度回転
    glutSolidCone(0.2, 0.7, 20, 10); // glutSolidCone(double radius, double height, int slices, int stacks)
    glPopMatrix();

    glPopMatrix();

    // 右腕
    glPushMatrix();
    glTranslatef(xx + 0.7, yy - 1.4 + height, 0.0);
    glRotatef(50.0, 0.0, 1.0, 0.0);
    glutSolidCone(0.2, 1.7, 20, 10); // glutSolidCone(double radius, double height, int slices, int stacks)
    glPopMatrix();

    // 左腕
    glPushMatrix();
    glTranslatef(xx - 0.7, yy - 1.4 + height, 0.0);
    glRotatef(-50.0, 0.0, 1.0, 0.0);
    glutSolidCone(0.2, 1.7, 20, 10); // glutSolidCone(double radius, double height, int slices, int stacks)
    glPopMatrix();

    // 右足
    glPushMatrix();
    glTranslatef(xx + 0.5, yy - 3.5 + height, -0.2);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glutSolidCone(0.2, 1.5, 20, 10); // glutSolidCone(double radius, double height, int slices, int stacks)
    glPopMatrix();

    // 左足
    glPushMatrix();
    glTranslatef(xx - 0.5, yy - 3.5 + height, -0.2);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glutSolidCone(0.2, 1.5, 20, 10); // glutSolidCone(double radius, double height, int slices, int stacks)
    glPopMatrix();

    // 新規パーツ1(帽子)
    glPushMatrix();
    glTranslatef(0.0 + xx, 0.8 + yy + height, 0.3);
    glRotatef(-90, 1.0, 0.0, 0.0);

    float difred[] = {1.0, 0.0, 0.0, 1.0}; // 赤の拡散反射
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difred);

    glutSolidCone(0.5, 1.0, 20, 10); // glutSolidCone(double radius, double height, int slices, int stacks)
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif); // 元の拡散反射に戻す

    // 新規パーツ2(帽子上部の球)
    glPushMatrix();
    glTranslatef(0.0 + xx, 1.8 + yy + height, 0.3);
    glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    glPopMatrix();

    glDisable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D); /* テクスチャを有効にする */

    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    /* テクスチャ座標とポリゴン座標を手動で設定 */
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-20.0, -10.0, -20.0); // hennkou
    glTexCoord2f(0.0, 20.0);
    glVertex3f(-20.0, -10.0, 20.0);
    glTexCoord2f(20.0, 20.0);

    glVertex3f(20.0, -10.0, 20.0); //
    glTexCoord2f(20.0, 0.0);
    glVertex3f(20.0, -10.0, -20.0); //
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void timer(int val)
{
    // float v_diff;

    if (flagj == 1)
    {
        vzt = vzt + gravt * dt;     // 重力による加速度
        height = height + vzt * dt; // heightの更新
        if (height < 0.0)
        {                 // 地面についた時に
            height = 0.0; // 高さ０に
            vzt = 0.0;    // 速度０に
            flagj = 0;    // ジャンプ終了
        }
    }

    glutPostRedisplay(); /* ディスプレイコールバック関数(display)の実行 */

    glutTimerFunc(50, timer, 1); /* Timer を再セットする */
}

void idle(void)
{
    if (flagh == 1)
    {
        headparam = fmod(headparam + 2.0, 360.0); /* 回転角を0～360度まで，2度ずつ増加 */
        if (headparam < epsilon)
            flagh = 0; /* 一周したら，flagを0にして回転を止める */
    }
    glutPostRedisplay();
}

void idle2(void)
{
    if (flagm == 1)
    {
        headparam = fmod(headparam + 2.0, 360.0); /* 回転角を0～360度まで，2度ずつ増加 */
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    { /* マウスがクリックされて */
        switch (button)
        {
        case GLUT_LEFT_BUTTON: /*クリックされたのが左ボタンだったら */
            flagm = 1;          /* 左ボタンがクリックされたときに行う処理 */
            break;
        case GLUT_RIGHT_BUTTON: /*クリックされたのが右ボタンだったら */
            flagm = 0;           /* 右ボタンがクリックされたときに行う処理 */
            break;
        }
    }
}


void keyboard(unsigned char key, int x, int y)
{
    static float col[4][4] =
        {{1.0, 0.0, 0.0, 1.0},
         {0.0, 1.0, 0.0, 1.0},
         {0.0, 0.0, 1.0, 1.0},
         {1.0, 1.0, 1.0, 1.0}};
    static int index = 0;
    static int smooth = 0;
    switch (key)
    {
    case 'q': /* qが入力されたら，プログラムを終了する */
        exit(0);
    case 'r':
        headparam += 30.0;
        break;
    case 'c':
        index++;
        index %= 4;
        glMaterialfv(GL_FRONT, GL_DIFFUSE, col[index]);
        break;
    case 's':
        if (smooth)
            glShadeModel(GL_SMOOTH);
        else
            glShadeModel(GL_FLAT);
        smooth = !smooth;
        break;
    case 'h': // hが入力されたらy軸周りに1回転
        flagh = 1;
        break;

    case 'j':
        vzt = 10; // 垂直方向の速度の設定
        flagj = 1;
        break;

    case 'i':
        z -= 0.5; // 距離減少
        if (z < 5.0)
            z = 5.0; // 制限
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();//変換行列の初期化
        gluLookAt(0.0, 3.0, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glutPostRedisplay();
        break;

    case 'o':
        z += 0.5; // 距離増加
        if (z > 20.0)
            z = 20.0; // 制限
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0, 3.0, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glutPostRedisplay();
        break;

    case 'f':
        z -= 0.5; // 距離減少
        if (z < 5.0)
            z = 5.0; // 制限
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // 背面から見ているのでx成分,z成分の値にマイナスをつける
        gluLookAt(0.0, 3.0, -z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glutPostRedisplay();
        break;

    case 'b':
        z += 0.5; // 距離増加
        if (z > 20.0)
            z = 20.0; // 制限
        glLoadIdentity();

        // 背面から見ているのでx,zを反転
        gluLookAt(0.0, 3.0, -z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glutPostRedisplay();
        break;

    default:
        break;
    }
}

void mousedrag(int x, int y)
{
    static float scale_x = 0.01; // 画面の座標からロボの座標に変えるためのスケールの変数）
    static float scale_y = 0.01;

    // マウスの座標をもとに位置も更新
    xx1 = x + XMIN;
    yy1 = YMAX - y;

    // xx, yyがマウスに伴って更新されるように
    xx -= (xx0 - xx1) * scale_x;
    yy -= (yy0 - yy1) * scale_y;

    // 現マウス位置を次回の基準に
    xx0 = xx1;
    yy0 = yy1;

    // 再描画
    glutPostRedisplay();
}

void initTexture1() /* テクスチャの初期設定 */
{
    makeImage1();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); /* テクスチャを1バイトずつ読み込み */
    /* テクスチャの繰り返し方の設定 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); /* テクスチャを繰り返す */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*テクスチャの拡大・縮小の設定  */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /* 最近傍の画素を元に拡大 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /* 最近傍の画素を元に縮小 */

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image1); /* テクスチャの割り当て */
}

void initLighting() /* 光源の属性の設定 */
{

    float diffuse_light[] = {0.5, 0.5, 0.5, 1.0};  /* 光源の拡散反射成分の強度 */
    float specular_light[] = {0.5, 0.5, 0.5, 1.0}; /* 光源の鏡面反射成分の強度 */
    float ambient_light[] = {0.5, 0.5, 0.5, 1.0};  /* 光源の環境光成分の強度 */

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);   /* 拡散反射成分の強度 */
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light); /* 鏡面反射成分の強度 */
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);   /* 環境光成分の強度 */
}

void init(char *winname)
{
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); /* zバッファ利用の宣言 */
    glutCreateWindow(winname);

    glClearColor(0.2, 0.4, 0.4, 1.0);

    glMatrixMode(GL_PROJECTION); /* 投影法に関する座標変換を開始 */
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 50.0); // ビューボリュームの上下の開き角をfovy=45に変更
    glMatrixMode(GL_MODELVIEW);           /* 視点の移動やモデルの移動など投影法
                                             以外の座標変換を開始 */
    gluLookAt(3.0, 4.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glShadeModel(GL_FLAT); /* シェーディングの種類をフラットシェーディングに設定 */
    glEnable(GL_LIGHT0);   /* LIGHT0の光源を有効にする */

    glEnable(GL_DEPTH_TEST); /* 隠面処理の開始を宣言 */

    initLighting();
    initTexture1();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    init(argv[0]);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse); /* マウス入力のコールバック関数mouseの指定 */
    glutIdleFunc(idle);
    glutIdleFunc(idle2);
    glutMotionFunc(mousedrag);   /* マウス左ボタンを押しながらドラッグする場合 */
    glutTimerFunc(50, timer, 1); /* Timer をセットする */
    glutMainLoop();
    return 0;
}
