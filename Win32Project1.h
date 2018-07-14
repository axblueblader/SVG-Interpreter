#pragma once
#ifndef _FUNC_H_
#define _FUNC_H_
#include "stdafx.h"
#include <objidl.h>
#include "resource.h"
#include <iostream>
#include <fstream>
#include <string>
#include <gdiplus.h>
#include <objidl.h>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#include <gdiplusgraphics.h>
using namespace std;
using namespace Gdiplus;
using namespace rapidxml;
// graphic.DrawRectangle(shape->get("x").retInt,shape->get("y").retInt,shape->get("width").retInt,shape->get("height").retInt);
// 
// graphic.DrawRectangle(x,y,width,height);
class Data
{
public:
	Data() {};
	~Data() {};
	int retInt()
	{
		return intval;
	};
	float retFloat()
	{
		return flval;
	};
	vector<int> retVec()
	{
		return intvec;
	};
	vector<float> retFvec()
	{
		return fvec;
	}
	vector<Point>retPoints()
	{
		return pointvec;
	}
	char* retString()
	{
		return str;
	}
private:
	int intval;
	float flval;
	vector<pair<int, int>> pointsval;
	vector<int> intvec;
	vector<float> fvec;
	vector<Point> pointvec;
	char str[100];
	// transformcontent
	friend class IntType;
	friend class FloatType;
	friend class RGBType;
	friend class PointType;
	friend class StringType;
	friend class ContentType;
	// friend class PointsType;
};

class Attribute
{
public:
	Attribute()
	{
	};
	~Attribute()
	{
	};
	virtual Data getVal() = 0;
	virtual char* getName() = 0;
	virtual void setVal(char*) = 0;
protected:
	char valName[20];
};

class IntType : public Attribute
{
public:
	IntType(char* name)
	{
		strcpy_s(valName, 20, name);
		x = 0;
	};
	IntType(int val)
	{
		x = val;
	};
	Data getVal()
	{
		Data tmp;
		tmp.intval = x;
		return tmp;
	};
	char* getName()
	{
		return valName;
	};
	void setVal(char* buf)
	{
		x = atoi(buf);
	};
private:
	int x;
};

class FloatType : public Attribute
{
public:
	FloatType(char* name)
	{
		strcpy_s(valName, 20, name);
		x = 1;
	};
	Data getVal()
	{
		Data tmp;
		tmp.flval = x;
		return tmp;
	};
	char* getName()
	{
		return valName;
	};
	void setVal(char* buf)
	{
		x = atof(buf);
	};
private:
	float x;
};

class RGBType : public Attribute
{
private:
	vector<int> intvec;
public:
	RGBType(char* name)
	{
		strcpy_s(valName, 20, name);
		intvec.push_back(0);
		intvec.push_back(0);
		intvec.push_back(0);
	}
	char* getName()
	{
		return valName;
	}
	Data getVal()
	{
		Data tmp;
		tmp.intvec = intvec;
		return tmp;
	};
	void setVal(char* attr_value)
	{
		string av = attr_value;
		replace(av.begin(), av.end(), ',', ' ');
		replace(av.begin(), av.end(), '(', ' ');
		replace(av.begin(), av.end(), ')', ' ');
		stringstream ss(av);
		string tmp;
		intvec.clear();
		while (ss >> tmp)
			if (tmp != "rgb")
				intvec.push_back(stoi(tmp));
	}
};

class ContentType : public Attribute
{
public:
	ContentType(char* name)
	{
		strcpy_s(valName, 20, name);
		for (size_t i = 0; i < 8; i++)
		{
			transform.push_back(0);
		}
	};
	Data getVal()
	{
		Data tmp;
		tmp.fvec.clear();
		tmp.fvec = transform;
		return tmp;
	};
	char* getName()
	{
		return valName;
	};
	void setVal(char* buf)
	{
		// parse
		string av = buf;
		replace(av.begin(), av.end(), ',', ' ');
		replace(av.begin(), av.end(), '(', ' ');
		replace(av.begin(), av.end(), ')', ' ');
		stringstream ss(av);
		string tmp;
		float order = 5;
		vector<string> strvec;
		while (ss >> tmp)
		{
			strvec.push_back(tmp);
		}
		int n = strvec.size();
		for (size_t i = 0; i < n; i++)
		{
			if (strvec[i] == "translate")
			{
				int t = 1;
				for (size_t x = 0; x < 2; x++)
				{
					transform[x] = stof(strvec[i + t]);
					t++;
				}
				transform[order] = 1;
				order++;
			}
			if (strvec[i] == "scale")
			{
				int t = 1;
				for (size_t x = 2; x < 4; x++)
				{
					if (strvec[i + t] != "translate" && strvec[i + 1] != "rotate")
					{
						transform[x] = stof(strvec[i + t]);
						t++;
					}
				}
				transform[order] = 2;
				order++;
			}
			if (strvec[i] == "rotate")
			{
				transform[4] = stof(strvec[i + 1]);
				transform[order] = 3;
				order++;
			}
		}
		/*
		while (ss >> tmp)
		{
		if (tmp == "translate")
		{
		for (size_t i = 0; i < 2; i++)
		{
		ss >> tmp;
		transform[i] = stof(tmp);
		}
		transform[order] = 1;
		order++;
		}
		if (tmp == "scale")
		{
		for (size_t i = 2; i < 4; i++)
		{
		ss >> tmp;
		if (tmp != "translate" && tmp != "rotate")
		{
		transform[i] = stof(tmp);
		}
		else
		{
		ss << tmp;
		}
		}
		transform[order] = 2;
		order++;
		}
		if (tmp == "rotate")
		{
		ss >> tmp;
		transform[4] = stof(tmp);
		transform[order] = 3;
		order++;
		}
		}*/
	};
private:
	vector<float> transform;
};

class PointType : public Attribute
{
public:
	PointType(char* name)
	{
		strcpy_s(valName, 20, name);
	};
	Data getVal()
	{
		Data tmp;
		tmp.pointvec = points;
		return tmp;
	};
	char* getName()
	{
		return valName;
	};
	void setVal(char* buf)
	{
		// parse
		string av = buf;
		replace(av.begin(), av.end(), ',', ' ');
		stringstream ss(av);
		string tmp;
		points.clear();
		bool switcher = true;
		int x, y;
		while (ss >> tmp)
		{
			if (switcher)
			{
				switcher = false;
				x = stoi(tmp);
			}
			else
			{
				switcher = true;
				y = stoi(tmp);
				points.push_back(Point(x, y));
			}
		}
	};
private:
	vector<Point> points;
};

class StringType : public Attribute
{
public:
	StringType(char* name)
	{
		strcpy_s(valName, 20, name);
	};
	Data getVal()
	{
		Data tmp;
		strcpy_s(tmp.str, 100, str);
		return tmp;
	};
	char* getName()
	{
		return valName;
	};
	void setVal(char* buf)
	{
		strcpy_s(str, 100, buf);
	};
private:
	char str[100];
};

void DoTransform(Graphics &graphics, vector<float> &transform)
{
	graphics.ResetTransform();
	// Default matrix order = Prepend
	/*if (transform[5] == 1)
	{
	// Rotate/scale before translate
	graphics.TranslateTransform(transform[0] * 1.0, transform[1] * 1.0);
	graphics.RotateTransform(transform[4] * 1.0);
	if (transform[2]*transform[3] != 0)
	{
	graphics.ScaleTransform(transform[2], transform[3]);
	}
	}
	else {
	// Translate before rotate/scale
	if (transform[2] * transform[3] != 0)
	{
	graphics.ScaleTransform(transform[2], transform[3]);
	}
	graphics.RotateTransform(transform[4] * 1.0);
	graphics.TranslateTransform(transform[0] * 1.0, transform[1] * 1.0);
	}
	*/
	for (size_t i = 5; i < 8; i++)
	{
		switch ((int)transform[i])
		{
		case 1: graphics.TranslateTransform(transform[0] * 1.0, transform[1] * 1.0);
			break;
		case 2: if (transform[2] * transform[3] != 0)
		{
			graphics.ScaleTransform(transform[2], transform[3]);
		}
				break;
		case 3:graphics.RotateTransform(transform[4] * 1.0);
			break;
		default:
			break;
		}
	}
};

namespace UserDef
{


	class Shape
	{
	public:
		Shape()
		{
		};
		virtual void setAttribute(char*, char*) = 0;
		static Shape* makeObject(char* className)
		{
			int fs = flyweights.size();
			for (int i = 0; i < fs; i++)
			{
				if (flyweights[i] != NULL)
				{
					if (!strcmp(flyweights[i]->className(), className))
					{
						return	flyweights[i]->Clone();
					}
				}
			}

			return NULL;
		};
		static void createFlyweight(Shape* shape)
		{
			if (shape == NULL)
			{
				return;
			}
			int fs = flyweights.size();
			for (int i = 0; i < fs; i++)
			{
				if (flyweights[i] != NULL)
				{
					if (flyweights[i]->className() == shape->className())
					{
						return;
					}
				}
			}
			flyweights.push_back(shape);
		};
		virtual char* className() = 0;
		virtual Shape* Clone() = 0;
		virtual void Draw(HDC hdc) = 0;
		virtual void setContent(xml_node<>*) {};
		~Shape() {
			flyweights.clear();
			attributes.clear();
		};
	protected:
		static vector<Shape*> flyweights; // contains unique prototype for all shapes
		vector<Attribute*> attributes;
	};

	vector<Shape*> Shape::flyweights;

	class Recta : public Shape
	{
	public:
		Recta()
		{
			strcpy_s(clsname, 20, "rect");
			Attribute* tmp;
			tmp = new IntType("x");
			attributes.push_back(tmp);
			tmp = new IntType("y");
			attributes.push_back(tmp);
			tmp = new IntType("width");
			attributes.push_back(tmp);
			tmp = new IntType("height");
			attributes.push_back(tmp);
			tmp = new RGBType("fill");
			attributes.push_back(tmp);
			tmp = new RGBType("stroke");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-width");
			tmp->setVal("0");
			attributes.push_back(tmp);
			tmp = new FloatType("fill-opacity");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-opacity");
			attributes.push_back(tmp);
			tmp = new ContentType("transform");
			attributes.push_back(tmp);
		};
		Recta(Recta& other)
		{
			strcpy_s(this->clsname, 20, other.clsname);
			int n = other.attributes.size();
			this->attributes.resize(n);
			for (int i = 0; i < n; i++)
				this->attributes[i] = other.attributes[i];
		};
		~Recta()
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				delete attributes[i];
			attributes.clear();
		};
		void setAttribute(char* attrname, char* attrval)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					attributes[i]->setVal(attrval);
		};
		Data get(char* attrname) {
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					return attributes[i]->getVal();
			return Data();
		};
		void Draw(HDC hdc)
		{
			Graphics graphics(hdc);

			// Create a Pen object.
			vector<int> rgb = this->get("stroke").retVec();
			Color col = Color(this->get("stroke-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]);
			Pen pen(col, this->get("stroke-width").retFloat());
			//Pen pen(Color(255, 0, 0, 0), 3);

			// Define the rectangle.
			int x = this->get("x").retInt();
			int y = this->get("y").retInt();
			int width = this->get("width").retInt();
			int height = this->get("height").retInt();
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);

			//transform
			vector<float> transform = this->get("transform").retFvec();
			DoTransform(graphics, transform);




			//Fill the rectangle
			rgb = this->get("fill").retVec();
			SolidBrush brush(Color(this->get("fill-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]));
			graphics.FillRectangle(&brush, x, y, width, height);

			// Draw the rectangle.
			if (this->get("stroke-width").retFloat() != 0)
				graphics.DrawRectangle(&pen, x, y, width, height);

			//graphics.ResetTransform();
			//graphics.ScaleTransform(1.75f, 0.5f);
			//graphics.RotateTransform(28.0f);
			//graphics.TranslateTransform(150.0f, 150.0f);


		}
		char* className()
		{
			return clsname;
		};
		Shape* Clone()
		{
			Shape* a = new Recta();
			return a;
		};
	private:
		char clsname[20];
	};

	class Line : public Shape
	{
	public:
		Line()
		{
			strcpy_s(clsname, 20, "line");
			Attribute* tmp;
			tmp = new IntType("x1");
			attributes.push_back(tmp);
			tmp = new IntType("y1");
			attributes.push_back(tmp);
			tmp = new IntType("x2");
			attributes.push_back(tmp);
			tmp = new IntType("y2");
			attributes.push_back(tmp);
			tmp = new RGBType("stroke");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-width");
			tmp->setVal("0");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-opacity");
			attributes.push_back(tmp);
			tmp = new ContentType("transform");
			attributes.push_back(tmp);
		};
		Line(Line& other)
		{
			strcpy_s(this->clsname, 20, other.clsname);
			int n = other.attributes.size();
			this->attributes.resize(n);
			for (int i = 0; i < n; i++)
				this->attributes[i] = other.attributes[i];
		};
		~Line()
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				delete attributes[i];
			attributes.clear();
		};
		void setAttribute(char* attrname, char* attrval)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					attributes[i]->setVal(attrval);
		};
		Data get(char* attrname)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					return attributes[i]->getVal();
			return Data();
		};
		void Draw(HDC hdc)
		{
			Graphics graphics(hdc);

			// Create a Pen object.
			vector<int> rgb = this->get("stroke").retVec();
			Color col = Color(this->get("stroke-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]);
			Pen pen(col, this->get("stroke-width").retFloat());
			//Pen Pen(Color(255, 0, 0, 0), 3);

			// Define
			int x1 = this->get("x1").retInt();
			int y1 = this->get("y1").retInt();
			int x2 = this->get("x2").retInt();
			int y2 = this->get("y2").retInt();
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);

			//transform
			vector<float> transform = this->get("transform").retFvec();
			DoTransform(graphics, transform);

			// Draw
			if (this->get("stroke-width").retFloat() != 0)
				graphics.DrawLine(&pen, x1, y1, x2, y2);

			//transform
		}
		char* className()
		{
			return clsname;
		};
		Shape* Clone()
		{
			Shape* a = new Line();
			return a;
		};
	private:
		char clsname[20];
	};

	class Circle : public Shape
	{
	public:
		Circle()
		{
			strcpy_s(clsname, 20, "circle");
			Attribute* tmp;
			tmp = new IntType("cx");
			attributes.push_back(tmp);
			tmp = new IntType("cy");
			attributes.push_back(tmp);
			tmp = new IntType("r");
			attributes.push_back(tmp);
			tmp = new RGBType("fill");
			attributes.push_back(tmp);
			tmp = new RGBType("stroke");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-width");
			tmp->setVal("0");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-opacity");
			attributes.push_back(tmp);
			tmp = new FloatType("fill-opacity");
			attributes.push_back(tmp);
			tmp = new ContentType("transform");
			attributes.push_back(tmp);
		};
		Circle(Circle& other)
		{
			strcpy_s(this->clsname, 20, other.clsname);
			int n = other.attributes.size();
			this->attributes.resize(n);
			for (int i = 0; i < n; i++)
				this->attributes[i] = other.attributes[i];
		};
		~Circle()
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				delete attributes[i];
			attributes.clear();
		};
		void setAttribute(char* attrname, char* attrval)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					attributes[i]->setVal(attrval);
		};
		Data get(char* attrname)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					return attributes[i]->getVal();
			return Data();
		};
		void Draw(HDC hdc)
		{
			Graphics graphics(hdc);

			// Create a Pen object.
			vector<int> rgb = this->get("stroke").retVec();
			Color col = Color(this->get("stroke-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]);
			Pen pen(col, this->get("stroke-width").retFloat());
			//Pen pen(Color(255, 0, 0, 0), 10);

			// Define
			int cx = this->get("cx").retInt();
			int cy = this->get("cy").retInt();
			int r = this->get("r").retInt();
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);

			//transform
			vector<float> transform = this->get("transform").retFvec();
			DoTransform(graphics, transform);

			//Fill
			rgb = this->get("fill").retVec();
			SolidBrush brush(Color(this->get("fill-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]));
			graphics.FillEllipse(&brush, cx - r, cy - r, r * 2, r * 2);

			// Draw
			if (this->get("stroke-width").retFloat() != 0)
				graphics.DrawEllipse(&pen, cx - r, cy - r, r * 2, r * 2);

			//transform
		}
		char* className()
		{
			return clsname;
		};
		Shape* Clone()
		{
			Shape* a = new Circle();
			return a;
		};
	private:
		char clsname[20];
	};

	class Ellipse : public Shape
	{
	public:
		Ellipse()
		{
			strcpy_s(clsname, 20, "ellipse");
			Attribute* tmp;
			tmp = new IntType("cx");
			attributes.push_back(tmp);
			tmp = new IntType("cy");
			attributes.push_back(tmp);
			tmp = new IntType("rx");
			attributes.push_back(tmp);
			tmp = new IntType("ry");
			attributes.push_back(tmp);
			tmp = new RGBType("fill");
			attributes.push_back(tmp);
			tmp = new RGBType("stroke");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-width");
			tmp->setVal("0");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-opacity");
			attributes.push_back(tmp);
			tmp = new FloatType("fill-opacity");
			attributes.push_back(tmp);
			tmp = new ContentType("transform");
			attributes.push_back(tmp);
		};
		Ellipse(Ellipse& other)
		{
			strcpy_s(this->clsname, 20, other.clsname);
			int n = other.attributes.size();
			this->attributes.resize(n);
			for (int i = 0; i < n; i++)
				this->attributes[i] = other.attributes[i];
		};
		~Ellipse()
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				delete attributes[i];
			attributes.clear();
		};
		void setAttribute(char* attrname, char* attrval)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					attributes[i]->setVal(attrval);
		};
		Data get(char* attrname)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					return attributes[i]->getVal();
			return Data();
		};
		void Draw(HDC hdc)
		{
			Graphics graphics(hdc);

			// Create a Pen object.
			vector<int> rgb = this->get("stroke").retVec();
			Color col = Color(this->get("stroke-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]);
			Pen pen(col, this->get("stroke-width").retFloat());
			//Pen Pen(Color(255, 0, 0, 0), 3);

			// Define
			int cx = this->get("cx").retInt();
			int cy = this->get("cy").retInt();
			int rx = this->get("rx").retInt();
			int ry = this->get("ry").retInt();
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);

			//transform
			vector<float> transform = this->get("transform").retFvec();
			DoTransform(graphics, transform);

			//Fill
			rgb = this->get("fill").retVec();
			SolidBrush brush(Color(this->get("fill-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]));
			graphics.FillEllipse(&brush, cx - rx, cy - ry, rx * 2, ry * 2);

			// Draw
			if (this->get("stroke-width").retFloat() != 0)
				graphics.DrawEllipse(&pen, cx - rx, cy - ry, rx * 2, ry * 2);

			//transform
		}
		char* className()
		{
			return clsname;
		};
		Shape* Clone()
		{
			Shape* a = new Ellipse();
			return a;
		};
	private:
		char clsname[20];
	};

	class Polyline : public Shape
	{
	public:
		Polyline()
		{
			strcpy_s(clsname, 20, "polyline");
			Attribute* tmp;
			tmp = new PointType("points");
			attributes.push_back(tmp);
			tmp = new RGBType("stroke");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-width");
			tmp->setVal("0");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-opacity");
			attributes.push_back(tmp);
			tmp = new RGBType("fill");
			attributes.push_back(tmp);
			tmp = new FloatType("fill-opacity");
			attributes.push_back(tmp);
			tmp = new ContentType("transform");
			attributes.push_back(tmp);
		};
		~Polyline()
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				delete attributes[i];
			attributes.clear();
		};
		void setAttribute(char* attrname, char* attrval)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					attributes[i]->setVal(attrval);
		};
		Data get(char* attrname)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					return attributes[i]->getVal();
			return Data();
		};
		void Draw(HDC hdc)
		{
			Graphics graphics(hdc);
			GraphicsPath gpath;
			// Create a Pen object.
			vector<int> rgb = this->get("stroke").retVec();
			Color col = Color(this->get("stroke-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]);
			Pen pen(col, this->get("stroke-width").retFloat());
			//Pen Pen(Color(255, 0, 0, 0), 3);

			// Define
			Point* tmp = (this->get("points").retPoints()).data();
			gpath.AddLines(tmp, this->get("points").retPoints().size());
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);

			//transform
			vector<float> transform = this->get("transform").retFvec();
			DoTransform(graphics, transform);

			//Fill
			rgb = this->get("fill").retVec();
			SolidBrush brush(Color(this->get("fill-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]));
			graphics.FillPath(&brush, &gpath);
			// Draw
			if (this->get("stroke-width").retFloat() != 0)
			{
				graphics.DrawPath(&pen, &gpath);
			}
			//transform
		}
		char* className()
		{
			return clsname;
		};
		Shape* Clone()
		{
			Shape* a = new Polyline();
			return a;
		};
	private:
		char clsname[20];
	};

	class Polygon : public Shape
	{
	public:
		Polygon()
		{
			strcpy_s(clsname, 20, "polygon");
			Attribute* tmp;
			tmp = new PointType("points");
			attributes.push_back(tmp);
			tmp = new RGBType("fill");
			attributes.push_back(tmp);
			tmp = new RGBType("stroke");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-width");
			tmp->setVal("0");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-opacity");
			attributes.push_back(tmp);
			tmp = new FloatType("fill-opacity");
			attributes.push_back(tmp);
			tmp = new ContentType("transform");
			attributes.push_back(tmp);
		};
		Polygon(Polygon& other)
		{
			strcpy_s(this->clsname, 20, other.clsname);
			int n = other.attributes.size();
			this->attributes.resize(n);
			for (int i = 0; i < n; i++)
				this->attributes[i] = other.attributes[i];
		};
		~Polygon()
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				delete attributes[i];
			attributes.clear();
		};
		void setAttribute(char* attrname, char* attrval)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					attributes[i]->setVal(attrval);
		};
		Data get(char* attrname)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					return attributes[i]->getVal();
			return Data();
		};
		void Draw(HDC hdc)
		{
			Graphics graphics(hdc);

			// Create a Pen object.
			vector<int> rgb = this->get("stroke").retVec();
			Color col = Color(this->get("stroke-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]);
			Pen pen(col, this->get("stroke-width").retFloat());
			//Pen Pen(Color(255, 0, 0, 0), 3);

			// Define
			Point* tmp = (this->get("points").retPoints()).data();
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);

			//transform
			vector<float> transform = this->get("transform").retFvec();
			DoTransform(graphics, transform);

			//Fill
			rgb = this->get("fill").retVec();
			SolidBrush brush(Color(this->get("fill-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]));
			graphics.FillPolygon(&brush, tmp, this->get("points").retPoints().size());

			// Draw
			if (this->get("stroke-width").retFloat() != 0)
				graphics.DrawPolygon(&pen, tmp, this->get("points").retPoints().size());

			//transform
		}
		char* className()
		{
			return clsname;
		};
		Shape* Clone()
		{
			Shape* a = new Polygon();
			return a;
		};
	private:
		char clsname[20];
	};

	class Text : public Shape
	{
	public:
		Text()
		{
			strcpy_s(clsname, 20, "text");
			Attribute* tmp;
			tmp = new IntType("x");
			attributes.push_back(tmp);
			tmp = new IntType("y");
			attributes.push_back(tmp);
			tmp = new RGBType("fill");
			attributes.push_back(tmp);
			tmp = new FloatType("fill-opacity");
			attributes.push_back(tmp);
			tmp = new IntType("font-size");
			attributes.push_back(tmp);
			tmp = new StringType("string");
			attributes.push_back(tmp);
		};
		~Text()
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				delete attributes[i];
			attributes.clear();
		};
		void setAttribute(char* attrname, char* attrval)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					attributes[i]->setVal(attrval);
		};
		Data get(char* attrname)
		{
			int n = attributes.size();
			for (int i = 0; i < n; i++)
				if (!strcmp(attributes[i]->getName(), attrname))
					return attributes[i]->getVal();
			return Data();
		};
		void setContent(xml_node<>* root)
		{
			this->setAttribute("string", root->value());
		}
		void Draw(HDC hdc)
		{
			Graphics graphics(hdc);
			vector<int> rgb = this->get("fill").retVec();
			int x = this->get("x").retInt();
			int y = this->get("y").retInt();
			FontFamily  fontFamily(L"Times New Roman");
			int fontsize = this->get("font-size").retInt();
			Font        font(&fontFamily, fontsize, FontStyleRegular, UnitPixel);
			PointF      point(x*1.0, y*1.0 - fontsize);
			SolidBrush  solidBrush(Color(this->get("fill-opacity").retFloat() * 255, rgb[0], rgb[1], rgb[2]));
			string buf = string(this->get("string").retString());
			std::wstring wstr = std::wstring(buf.begin(), buf.end());
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);
			//wchar_t* wchar = wstr.c_str();

			//transform

			// Draw
			//(WCHAR)this->get("string").retString()
			//Font testfont(L"Arial", 30);
			//SolidBrush blackBrush(Color(255, 0, 0, 0));
			graphics.DrawString(wstr.c_str(), -1, &font, point, &solidBrush);


		}
		char* className()
		{
			return clsname;
		};
		Shape* Clone()
		{
			Shape* a = new Text();
			return a;
		};
	private:
		char clsname[20];
		char* val;
	};

	class Group : public Shape
	{
	public:
		Group() {
			strcpy_s(clsname, 20, "g");
			Attribute* tmp;
			tmp = new RGBType("fill");
			attributes.push_back(tmp);
			tmp = new RGBType("stroke");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-width");
			tmp->setVal("0");
			attributes.push_back(tmp);
			tmp = new FloatType("stroke-opacity");
			attributes.push_back(tmp);
			tmp = new FloatType("fill-opacity");
			attributes.push_back(tmp);
			tmp = new ContentType("transform");
			attributes.push_back(tmp);
		};
		void setAttribute(char* name, char* value)
		{
			int n = members.size();
			for (size_t i = 0; i < n; i++)
			{
				members[i]->setAttribute(name, value);
			}
		};
		void setContent(xml_node<>* root)
		{
			for (xml_node<>* i = root->first_node(); i; i = i->next_sibling())
			{
				Shape* a = a->makeObject(i->name());
				a->setContent(i);
				for (xml_attribute<> *at = i->first_attribute(); at; at = at->next_attribute())
				{
					if (a != NULL)
					{
						a->setAttribute(at->name(), at->value());
					}
				}
				members.push_back(a);
			}
		};
		void Draw(HDC hdc)
		{
			int n = members.size();
			for (size_t i = 0; i < n; i++)
			{
				members[i]->Draw(hdc);
			}
		};
		char* className()
		{
			return clsname;
		};
		Shape* Clone()
		{
			Shape* a = new Group();
			return a;
		};
	private:
		vector<Shape*> members;
		char clsname[20];
	};
}
#endif