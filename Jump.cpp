// Jump.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//adb devices
//adb shell screencap -p /sdcard/01.png
//adb pull /sdcard/01.png
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include<cmath>
using namespace cv;
using namespace std;

Mat image;

int 白点测算(int 原点x, int 原点y)
{
	for (int y = 500; y < 原点y; y++)
	{
		for (int x = 0; x < image.size().width; x++)
		{
			if (image.at<Vec3b>(y, x) == Vec3b(245, 245, 245))
			{
				int 距离 = sqrt(pow(x + 6.5f - 原点x, 2) + pow(y + 10.5f - 原点y, 2))*1.445f;
				cout << "距离获取成功:" << 距离 << endl;
				return  距离;
			}
		}
	}
	cout << "无法获取距离\n";
	return -1;
}
int 手动坐标(int 原点x, int 原点y, int 目标x, int 目标y)
{
	int 距离 = sqrt(pow(目标x - 原点x, 2) + pow(目标y - 原点y, 2))*1.46f;
	cout << "距离计算成功:" << 距离 << endl;
	return  距离;
}
int 顶点测算(int 原点x, int 原点y)
{
	int 上顶点x = image.size().width / 2, 上顶点y;
	int 侧顶点x, 侧顶点y;

	int 方向 = 原点x < 上顶点x ? -1 : 1;
	try
	{
		for (int y = 500; y < 原点y; y++)
		{
			Vec3b 地面颜色t = image.at<Vec3b>(y, 0);
			for (int x = 方向 == 1 ? 0 : image.size().width - 1; x != 上顶点x; x += 方向)
			{
				if (image.at<Vec3b>(y, x) != 地面颜色t)
				{
					y++;
					cout << "上顶点搜索成功：";
					cout << "【" << x << "," << y << "】\n";
					上顶点x = x;
					上顶点y = y;
					侧顶点x = x;
					侧顶点y = y;

					方向 = 原点x < 上顶点x ? -1 : 1;
					for (y += 1; y < 原点y; y++)
					{
						Vec3b 地面颜色t = image.at<Vec3b>(y, 0);
						for (int x = 方向 == 1 ? 0 : image.size().width - 1; x != 上顶点x; x += 方向)
						{
							if (image.at<Vec3b>(y, x) != 地面颜色t)
							{
								//cout << "【" << x << "," << y << "】\n";
								if (x*方向 < 侧顶点x*方向)
								{
									侧顶点x = x;
									侧顶点y = y;
									break;
								}
								else
								{
									cout << "侧顶点搜索成功：";
									cout << "【" << 侧顶点x << "," << 侧顶点y << "】\n";
									if (sqrt(pow(上顶点x - 侧顶点x, 2) + pow(上顶点y - 侧顶点y, 2)) < 66)
									{
										cout << "上侧顶点数据不正常\n";
										return  -1;
									}
									cout << "终点计算成功：";
									cout << "【" << 上顶点x << "," << 侧顶点y << "】\n";

									return  sqrt(pow(原点x - 上顶点x, 2) + pow(原点y - 侧顶点y, 2))*1.4f;
								}
							}
						}
					}
				}
			}
		}
		cout << "无法获取距离\n";
		return -1;
	}
	catch (const std::exception&)
	{
		cout << "无法获取距离\n";
		return -1;
	}
}

int 特殊修复(int 原点x, int 原点y, int 类型)
{
	switch (类型)
	{
	case 1:
		cout << "启动特殊修复1" << endl;
		int 方向 = 原点x < image.size().width / 2 ? -1 : 1;
		for (int y = 500; y < 原点y; y++)
		{
			Vec3b 地面颜色t = image.at<Vec3b>(y, 0);
			for (int x = 方向 == 1 ? 0 : image.size().width - 1; x != image.size().width / 2; x += 方向)
			{
				if (image.at<Vec3b>(y, x) != 地面颜色t)
				{
					cout << "猜测终点：";
					cout << "【" << x << "," << y + 86 << "】\n";
					int 距离 = sqrt(pow(原点x - x, 2) + pow(原点y - (y + 86), 2))*1.4f;
					cout << "距离获取成功:" << 距离 << endl;
					return  距离;
				}
			}
			break;
		}
	}
}

int main()
{
	int 原点x = 0;
	int 原点y = 0;
	while (true)
	{
		system("adb shell screencap -p /sdcard/01.png");
		system("adb pull /sdcard/01.png");
		image = imread("01.png");
		cout << "发现图片:" << image.size() << endl;
		cout << "\n";
		for (int y = 0; y < image.size().height; y++)
		{
			for (int x = 0; x < image.size().width; x++)
			{
				if (image.at <Vec3b>(y, x).val[2] == 52 && image.at <Vec3b>(y, x).val[1] == 53 && image.at <Vec3b>(y, x).val[0] == 59)
				{
					cout << "原点搜索成功";
					原点x = x + 2; 原点y = y + 189;
					cout << "【" << 原点x << "," << 原点y << "】\n";
					y = image.size().height;
					break;
				}
			}
		}

		string 跳跃命令 = "adb shell input swipe 1 1 1 1 ";
		int 距离 = 顶点测算(原点x, 原点y);

		//string 触摸时常 = "-1";

		switch (距离)
		{
		case -1:
			int 目标x;
			int 目标y;
			cout << "请输入x轴\n";
			cin >> 目标x;
			cout << "请输入y轴\n";
			cin >> 目标y;
			距离 = 手动坐标(原点x, 原点y, 目标x, 目标y);
			//continue;
			break;
		}

		cout << "\n距离获取成功:" << 距离 << endl;
		string 触摸时常 = to_string(距离);
		跳跃命令 += 触摸时常;
		system((char*)跳跃命令.data());
		cout << "\n\n#################\n\n";
		waitKey(1000);
	}
}