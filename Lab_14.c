#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GameLife(int** pict, int h, int w){
    int N;
    int** tmp = (int**)malloc(h * sizeof(int*));
    int i;
    for(i = 0; i < h; i++)
        tmp[i] = (int*)malloc(w * sizeof(int));
    int x, y;
    for(x = 0; x < w; x++)
        for(y = 0; y < h; y++)
            tmp[x][y] = pict[x][y];
    for(x = 0; x < w; x++){
        for(y = 0; y < h; y++){
        if (y == 0){
            if (x == 0) N = pict[0][1] + pict[1][0] + pict[1][1];
            if (x == w - 1) N = pict[w - 2][0] + pict[w - 2][1] + pict[w - 1][0];
            else N = pict[x-1][0] + pict[x + 1][0] + pict[x - 1][1] + pict[x][1] + pict[x + 1][1];
        }
        else if (y == h - 1){
            if (x == 0) N = pict[0][h -1] + pict[1][h -1] + pict[0][h -2];
            if (x == w - 1) N = pict[w - 2][h-1] + pict[w - 2][h - 2] + pict[w - 1][h - 2];
            else N = pict[x - 1][h -1] + pict[x - 1][h + 1] + pict[x][h -2] + pict[x - 1][h - 2] + pict[x+1][h -2];
        }
        else
            N = pict[x - 1][y + 1] + pict[x][y+1] + pict[x + 1][y + 1] + pict[x - 1][y] + pict[x+1][y] + pict[x - 1][y - 1] + pict[x][y - 1] + pict[x + 1][y - 1];
            if(pict[x][y] == 1){
                if(N == 2) tmp[x][y] = pict[x][y];
                if(N == 3) tmp[x][y] = pict[x][y];
                if(N > 3) tmp[x][y] = 0;
                if(N < 2) tmp[x][y] = 0;
            }
            else{
                if(N == 3) tmp[x][y] = 1;
            }
            N = 0;
        }
    }
    int sq = 0;
    for(x = 0; x < w; x++){
        for(y = 0; y < h; y++){
            if(pict[x][y] == tmp[x][y])
                sq++;
            pict[x][y] = tmp[x][y];
        }}
    if(sq == h * w)
        exit(0); //Выход из программы
}
struct bmp{
    int width;
    int height;
    int size;
};
int main(int argc, char* argv[])
{
    struct bmp image;
    unsigned char header[54];
    int i, j, l, m;
    int maxiter , dumpfreq = 1;
    char* dirname;
    FILE* file;
    for(i = 0; i < argc; i++){
        if(!strcmp("--input", argv[i])){
            file = fopen(argv[1 + 1], "rb");
        }
        if(!strcmp("--output", argv[i])){
            dirname = argv[i + 1];
        }
        if(!strcmp("--max_iter", argv[i])){
            maxiter = strtol(argv[i + 1], 0, 10);
        }
        if(!strcmp("--dump_freq", argv[i])){
            dumpfreq = strtol(argv[i + 1], 0, 10);
        }
    }
    //Size	4	14	Длина заголовка
    //Width	4	18	Ширина изображения, точки
    //Height 4	22	Высота изображения, точки
    fread(header, 1, 54, file);
    image.width = header[21] * 256 * 256 * 256 + header[20] * 256 * 256 + header[19] * 256 + header[18];
    image.height = header[25] * 256 * 256 * 256 + header[24] * 256 * 256 + header[23] * 256 + header[22];
    image.size = header[5] * 256 * 256 * 256 + header[4] * 256 * 256 + header[3] * 256 + header[2];
    unsigned char *imagebyte = malloc(sizeof(unsigned char) * (image.size - 54));
    fread(imagebyte, 1, image.size, file);
    int** img = (int**)malloc(image.height * sizeof(int*));
    for(i = 0; i < image.height; i++)
        img[i] = (int*)malloc(image.width * sizeof(int));
    int now;
    now = -(image.width % 4);
    for(i = image.height - 1; i >= 0; i--){
        now += (image.width % 4);
        for(j = 0; j < image.width; j++){
            if(imagebyte[now] == 255) //Если пиксель белый - то зануляем элемент в массиве
                img[i][j] = 0;
            else
                img[i][j] = 1;
            now += 3;
        }
    }
    for (l = 0; l < maxiter; l++){
        GameLife(img, image.height, image.width);
        if(l % dumpfreq == 0) {
            char* filename = (char*)malloc(l * sizeof(char));;
            char* way = (char*)malloc(100 * sizeof(char));
            strcpy(way, dirname);
            strcat(strcat(way, "\\"), strcat(itoa(l, filename, 10), ".bmp"));
            FILE* life = fopen(way, "w");
            fwrite(header, 1, 54, life);
            m = 0;
            for (i = image.height - 1; i >= 0; i--) {
                for (j = 0; j < image.width; j++){
                    for (now = 0; now < 3; now++){
                        if (img[i][j] == 1)
                            imagebyte[m] = 0;
                        else
                            imagebyte[m] = 255;
                        m++;
                    }
                }
                while (m % 4 != 0) {
                    imagebyte[m] = 0;
                    m++;
                }
            }
            fwrite(imagebyte, 1, image.size, life);
            fclose(life);
            free(filename);
            free(way);
        }
    }
    for(i = 0; i < image.height; i++) {

        free(img[i]);
    }
    free(img);
    free(imagebyte);
    return 0;
}
//C:\Users\Иван\Desktop\Proga\Lab_14.exe --input C:\Users\Иван\Desktop\Proga\Test.bmp --output C:\Users\Иван\Desktop\Proga\Test2 --max_iter 10 --dump_freq 2
