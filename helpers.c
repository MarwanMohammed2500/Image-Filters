#include "helpers.h"
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE gray_scale;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            gray_scale = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            image[i][j].rgbtRed = gray_scale;
            image[i][j].rgbtGreen = gray_scale;
            image[i][j].rgbtBlue = gray_scale;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE reflect_r, temp_r, reflect_g, temp_g, reflect_b, temp_b;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < ((width) / 2); j++)
        {
            // To save the actual values of the pixel, for some reason making only one temp value for one of the pixels didn't work
            // so now i have to make two, one for each side.
            reflect_r = image[i][width - 1 - j].rgbtRed, temp_r = image[i][j].rgbtRed,
            reflect_g = image[i][width - 1 - j].rgbtGreen, temp_g = image[i][j].rgbtGreen,
            reflect_b = image[i][width - 1 - j].rgbtBlue, temp_b = image[i][j].rgbtBlue;

            image[i][j].rgbtRed = reflect_r;
            image[i][width - 1 - j].rgbtRed = temp_r;

            image[i][j].rgbtGreen = reflect_g;
            image[i][width - 1 - j].rgbtGreen = temp_g;

            image[i][j].rgbtBlue = reflect_b;
            image[i][width - 1 - j].rgbtBlue = temp_b;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // So we don't modify the values of the original image just yet.
    RGBTRIPLE new_values[height][width];

    int sum_r, sum_g, sum_b;
    float count;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            count = 0.0;
            sum_r = 0;
            sum_g = 0;
            sum_b = 0;

            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (((y + i) >= 0 && (y + i) < height) && ((x + j) >= 0 && (x + j) < width))
                    {
                        sum_r += image[y + i][x + j].rgbtRed;
                        sum_g += image[y + i][x + j].rgbtGreen;
                        sum_b += image[y + i][x + j].rgbtBlue;
                        count++;
                    }
                }
            }

            // assigning each RGB value to the corresponding average value of neighbouring pixels
            new_values[y][x].rgbtRed = round(sum_r / count);
            new_values[y][x].rgbtGreen = round(sum_g / count);
            new_values[y][x].rgbtBlue = round(sum_b / count);
        }
    }
    // Now to modify the image itself.
    for (int k = 0; k < height; k++)
    {
        for (int l = 0; l < width; l++)
        {
            image[k][l].rgbtRed = new_values[k][l].rgbtRed;
            image[k][l].rgbtGreen = new_values[k][l].rgbtGreen;
            image[k][l].rgbtBlue = new_values[k][l].rgbtBlue;
        }
    }
}

// This is to make sure values don't exceed 255.
int cap(int val)
{
    if (val > 255)
    {
        val = 255;
    }
    return val;
}
// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // To put the modified pixels in.
    RGBTRIPLE new_values[height][width];

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    double Gx_r, Gx_g, Gx_b;
    double Gy_r, Gy_g, Gy_b;
    double G_r, G_g, G_b;

    // Does blacking out the edge of the image and corners not right?
    // for (int a = 0, b = 0; a < height && b < width; a++, b++)
    // {
    //     image[a][0].rgbtRed = 0, image[a][0].rgbtGreen = 0, image[a][0].rgbtBlue = 0,
    //     image[a][width - 1].rgbtRed = 0, image[a][width - 1].rgbtGreen = 0, image[a][width - 1].rgbtBlue = 0;

    //     image[0][b].rgbtRed = 0, image[0][b].rgbtGreen = 0, image[0][b].rgbtBlue = 0,
    //     image[height - 1][b].rgbtRed = 0, image[height - 1][b].rgbtGreen = 0, image[height - 1][b].rgbtBlue = 0;
    // }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Gx_r = 0, Gx_g = 0, Gx_b = 0;
            Gy_r = 0, Gy_g = 0, Gy_b = 0;

            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (((y + i) >= 0 && (y + i) < height) && ((x + j) >= 0 && (x + j) < width))
                    {
                        Gx_r += image[y + i][x + j].rgbtRed * Gx[i + 1][j + 1];
                        Gx_g += image[y + i][x + j].rgbtGreen * Gx[i + 1][j + 1];
                        Gx_b += image[y + i][x + j].rgbtBlue * Gx[i + 1][j + 1];

                        Gy_r += image[y + i][x + j].rgbtRed * Gy[i + 1][j + 1];
                        Gy_g += image[y + i][x + j].rgbtGreen * Gy[i + 1][j + 1];
                        Gy_b += image[y + i][x + j].rgbtBlue * Gy[i + 1][j + 1];
                    }
                }
            }

            G_r = cap(round(sqrt((Gx_r * Gx_r) + (Gy_r * Gy_r))));
            G_g = cap(round(sqrt((Gx_g * Gx_g) + (Gy_g * Gy_g))));
            G_b = cap(round(sqrt((Gx_b * Gx_b) + (Gy_b * Gy_b))));

            // So the original Values don't change as long as we are not done.
            new_values[y][x].rgbtRed = G_r;
            new_values[y][x].rgbtGreen = G_g;
            new_values[y][x].rgbtBlue = G_b;
        }
    }
    // After everything is done, this is to get the new values for each pixel on our original image.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = new_values[i][j].rgbtRed;
            image[i][j].rgbtGreen = new_values[i][j].rgbtGreen;
            image[i][j].rgbtBlue = new_values[i][j].rgbtBlue;
        }
    }
}