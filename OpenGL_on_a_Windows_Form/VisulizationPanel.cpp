#include "stdafx.h"
#include "VisualizationPanel.h"

namespace OpenGLForm{
	ReadCSV read_csv;
	Preprocessing_Data preprocessing_data;
	char quote[24][80];
	char title[6][200];
	wchar_t station_name[200][200];
	char five_minutes[12][80];
	vector<vector4> histogram_position_table;
	vector<int> histogram_index;
	vector< vector<vector4> > raw_data_position_table;
	vector<int> raw_data_index;
	vector < vector<int> > raw_data_index_2D;
	float scale_x[3],scale_y[3],scale_z[3];
	float scale_size[3],scale_factor[3];
	int last_X[3],last_Y[3];
	float move_x[3],move_y[3],move_z[3];
	int windowWidth[3],windowHeight[3];
	FTPixmapFont font(FONT_FILE);
	FTFont *font_new;
	FTFont *font_english;
	char* text;
	double ModelViewMatrix2[16];
	int select_histogram_index;
	bool select_histogram_flag;
	int counter;
	float test;

	VisualizationPanel::VisualizationPanel(Form ^ parentForm,Panel ^ parentPanel, GLsizei iWidth, GLsizei iHeight,ReadCSV read_csv_ref,Preprocessing_Data preprocessing_data_ref){
			CreateParams^ cp = gcnew CreateParams;	
			read_csv = read_csv_ref;
			preprocessing_data = preprocessing_data_ref;
			// Set the position on the form
			cp->X = 0;
			cp->Y = 0;
			cp->Height = iHeight;
			cp->Width = iWidth;

			// Specify the form as the parent.
			cp->Parent = parentForm->Handle;

			// Create as a child of the specified parent and make OpenGL compliant (no clipping)
			cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

			// Create the actual window
			this->CreateHandle(cp);
			//parentForm->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &COpenGL::COGL_MouseClick);
			//parentForm->MouseClick += gcnew System::Windows::Forms::MouseEventHandler( this, &COpenGL::panel1_MouseDown );
			//parentForm->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler( this, &COpenGL::COGL_Scroll );
			m_hDC = GetDC((HWND)parentPanel->Handle.ToPointer());

			this->parentForm = parentForm;/////////////////////////////

			if(m_hDC)
			{
				MySetPixelFormat(m_hDC);
				ReSizeGLScene(iWidth, iHeight);
				InitGL();		
			}

			rtri = 0.0f;
			rquad = 0.0f;

			//font_new = new FTGLTextureFont("kaiu.ttf");
			font_new = new FTGLTextureFont("simfang.ttf");
			font_new->CharMap(ft_encoding_unicode);

			font_english = new FTGLTextureFont("arial.ttf");
			font_english->CharMap(ft_encoding_unicode);

			StationNameInitialization();

			test = 0.0;

	}

	VisualizationPanel::~VisualizationPanel(System::Void){
		this->DestroyHandle();
	}


	GLint VisualizationPanel::MySetPixelFormat(HDC hdc){
		static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
				{
					sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
					1,											// Version Number
					PFD_DRAW_TO_WINDOW |						// Format Must Support Window
					PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
					PFD_DOUBLEBUFFER,							// Must Support Double Buffering
					PFD_TYPE_RGBA,								// Request An RGBA Format
					16,										// Select Our Color Depth
					0, 0, 0, 0, 0, 0,							// Color Bits Ignored
					0,											// No Alpha Buffer
					0,											// Shift Bit Ignored
					0,											// No Accumulation Buffer
					0, 0, 0, 0,									// Accumulation Bits Ignored
					16,											// 16Bit Z-Buffer (Depth Buffer)  
					0,											// No Stencil Buffer
					0,											// No Auxiliary Buffer
					PFD_MAIN_PLANE,								// Main Drawing Layer
					0,											// Reserved
					0, 0, 0										// Layer Masks Ignored
				};
			
			GLint  iPixelFormat; 
		 
			// get the device context's best, available pixel format match 
			if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
			{
				MessageBox::Show("ChoosePixelFormat Failed");
				return 0;
			}
			 
			// make that match the device context's current pixel format 
			if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
			{
				MessageBox::Show("SetPixelFormat Failed");
				return 0;
			}

			if((m_hglrc = wglCreateContext(m_hDC)) == NULL)
			{
				MessageBox::Show("wglCreateContext Failed");
				return 0;
			}

			if((wglMakeCurrent(m_hDC, m_hglrc)) == NULL)
			{
				MessageBox::Show("wglMakeCurrent Failed");
				return 0;
			}


			return 1;
	}
	bool VisualizationPanel::InitGL(GLvoid){
		glShadeModel(GL_SMOOTH);							// Enable smooth shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black background
		glClearDepth(1.0f);									// Depth buffer setup
		glEnable(GL_DEPTH_TEST);							// Enables depth testing
		glDepthFunc(GL_LEQUAL);								// The type of depth testing to do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations
		return TRUE;										// Initialisation went ok
	}
	GLvoid VisualizationPanel::ReSizeGLScene(GLsizei width, GLsizei height){
			if (height==0)										// Prevent A Divide By Zero By
			{
				height=1;										// Making Height Equal One
			}

			glViewport(0,0,width,height);						// Reset The Current Viewport

			glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
			glLoadIdentity();									// Reset The Projection Matrix

			// Calculate The Aspect Ratio Of The Window
			gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

			glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
			glLoadIdentity();									// Reset The Modelview Matrix
	}
	System::Void VisualizationPanel::DrawRectWithOpenGL(RECTANGLE* pRect,vector<float> draw_color){
		glPushMatrix();  //Make sure our transformations don't affect any other transformations in other code
		//glTranslatef(pRect->x, pRect->y,0);  //Translate rectangle to its assigned x and y position
			
		//glBegin(GL_TRIANGLES);
		glBegin(GL_QUADS);   //We want to draw a quad, i.e. shape with four sides
			glColor3f(draw_color[0],draw_color[1],draw_color[2]); //Set the colour to red 
			glVertex3f(pRect->x,pRect->y,0);
			glVertex3f(pRect->x+pRect->w,pRect->y,0);
			glVertex3f(pRect->x+pRect->w,pRect->y+pRect->h,0);
			glVertex3f(pRect->x,pRect->y+pRect->h,0);      
		glEnd();
		glPopMatrix();
	}

	System::Void VisualizationPanel::DrawRect(int x, int y, int h, int w, vector<float> draw_color)
	{
		RECTANGLE *rect;
		rect = new RECTANGLE();
		rect->h = h;
		rect->w = w;
		rect->x = x;
		rect->y = y;
		DrawRectWithOpenGL(rect,draw_color);
		delete rect;		
	}

	System::Void VisualizationPanel::DrawCircle(int x, int y, float radius, float r, float g, float b)
	{
		int i;
		int triangleAmount = 30; //# of triangles used to draw circle
	
		//GLfloat radius = 0.8f; //radius
		GLfloat twicePi = 2.0f * 3.14;
	
		glPushMatrix(); 
		glBegin(GL_TRIANGLE_FAN);
			glColor3f(r,g,b);
			glVertex2f(x, y); // center of circle
			for(i = 0; i <= triangleAmount;i++)
			{ 
				glVertex2f(
					x + (radius * cos(i *  twicePi / triangleAmount)), 
					y + (radius * sin(i * twicePi / triangleAmount))
				);
			}
		glEnd();
		glPopMatrix();
	}

	System::Void VisualizationPanel::drawHallowCircle(GLfloat x, GLfloat y, GLfloat r, float R, float G, float B) {
		static const double inc = 3.14 / 12.0;
		static const double max = 2 * 3.14;

		
		glEnable(GL_LINE_SMOOTH);
		glPushMatrix();

		glLineWidth(1.0);
		//glColor3f( R, G, B );
		glColor3f( 0.0, 0.0, 0.0 );
		glBegin(GL_LINE_LOOP);
		for(double d = 0; d < max; d += inc) {
			glVertex2f(cos(d) * r + x, sin(d) * r + y);
		}
		glEnd();
		glPopMatrix();
	}

	System::Void VisualizationPanel::DrawLine(float line_width, float x1, float y1, float x2, float y2)
	{
		glLineWidth(line_width);

		glPushMatrix();

		glColor3f( 1.0f, 1.0, 1.0f );

		glBegin(GL_LINES);
		glVertex2d(x1, y1 ); 
		glVertex2d(x2, y2 ); 
		glEnd();

		glPopMatrix();
	}

	int VisualizationPanel::DrawTextOfStationName(int n, int x, int y, int scale)
	{
		//wcscpy(station_name[0],L"Weekdend Variance");
		
		glPushMatrix();

		font_new->FaceSize(scale);
		glTranslatef(x, y, 0);
		//glRotatef(270, 0.0, 0.0, -1.0);
		//glRotatef(180, 0.0, 1.0, 0.0);
		glRotatef(180, 1.0, 0.0, 0.0);
		//wchar_t strText[] = L"快給我中文Please"; 
		glColor3f(1.0, 1.0, 1.0);
		//font_new->Render(strText);
		font_new->Render(station_name[n]);

		glPopMatrix();

		return wcslen(station_name[n]);
	}

	System::Void VisualizationPanel::SwapOpenGLBuffers(System::Void){
		SwapBuffers(m_hDC);		
	}
	System::Void VisualizationPanel::wglmakecur(){
		wglMakeCurrent(m_hDC,m_hglrc);
	}

	double VisualizationPanel::round(double f)
	{
		int pp = 2;
		return int(f * pow(10.0, pp) + .5) /  pow(10.0, pp);
	}

	System::Void VisualizationPanel::StationNameInitialization()
	{
		wcscpy(station_name[0],L"");
		wcscpy(station_name[1],L"");
		wcscpy(station_name[2],L"");
		wcscpy(station_name[3],L"");
		wcscpy(station_name[4],L"");
		wcscpy(station_name[5],L"");
		wcscpy(station_name[6],L"");
		wcscpy(station_name[7],L"松山機場");
		wcscpy(station_name[8],L"中山國中");
		wcscpy(station_name[9],L"南京東路");
		wcscpy(station_name[10],L"忠孝復興(文)");
		//
		wcscpy(station_name[11],L"大安");
		wcscpy(station_name[12],L"科技大樓");
		wcscpy(station_name[13],L"六張犁");
		wcscpy(station_name[14],L"麟光");
		wcscpy(station_name[15],L"辛亥");
		wcscpy(station_name[16],L"萬芳醫院");
		wcscpy(station_name[17],L"萬芳社區");
		wcscpy(station_name[18],L"木柵");
		wcscpy(station_name[19],L"動物園");
		wcscpy(station_name[20],L"");
		//
		wcscpy(station_name[21],L"大直");
		wcscpy(station_name[22],L"劍南路");
		wcscpy(station_name[23],L"西湖");
		wcscpy(station_name[24],L"港墘");
		wcscpy(station_name[25],L"文德");
		wcscpy(station_name[26],L"內湖");
		wcscpy(station_name[27],L"大湖公園");
		wcscpy(station_name[28],L"葫洲");
		wcscpy(station_name[29],L"東湖");
		wcscpy(station_name[30],L"南港軟體園區");
		//
		wcscpy(station_name[31],L"南港展覽館(文)");
		wcscpy(station_name[32],L"小碧潭");
		wcscpy(station_name[33],L"新店");
		wcscpy(station_name[34],L"新店區公所");
		wcscpy(station_name[35],L"七張");
		wcscpy(station_name[36],L"大坪林");
		wcscpy(station_name[37],L"景美");
		wcscpy(station_name[38],L"萬隆");
		wcscpy(station_name[39],L"公館");
		wcscpy(station_name[40],L"台電大樓");
		//
		wcscpy(station_name[41],L"古亭");
		wcscpy(station_name[42],L"中正紀念堂");
		wcscpy(station_name[43],L"小南門");
		wcscpy(station_name[44],L"");
		wcscpy(station_name[45],L"頂溪");
		wcscpy(station_name[46],L"永安市場");
		wcscpy(station_name[47],L"景安");
		wcscpy(station_name[48],L"南勢角");
		wcscpy(station_name[49],L"");
		wcscpy(station_name[50],L"臺大醫院");
		//
		wcscpy(station_name[51],L"台北車站(板)");
		wcscpy(station_name[52],L"台北車站(淡)");
		wcscpy(station_name[53],L"中山");
		wcscpy(station_name[54],L"雙連");
		wcscpy(station_name[55],L"民權西路(淡)");
		wcscpy(station_name[56],L"圓山");
		wcscpy(station_name[57],L"劍潭");
		wcscpy(station_name[58],L"士林");
		wcscpy(station_name[59],L"芝山");
		wcscpy(station_name[60],L"明德");
		//
		wcscpy(station_name[61],L"石牌");
		wcscpy(station_name[62],L"唭哩岸");
		wcscpy(station_name[63],L"奇岩");
		wcscpy(station_name[64],L"北投");
		wcscpy(station_name[65],L"新北投");
		wcscpy(station_name[66],L"復興崗");
		wcscpy(station_name[67],L"忠義");
		wcscpy(station_name[68],L"關渡");
		wcscpy(station_name[69],L"竹圍");
		wcscpy(station_name[70],L"紅樹林");
		//
		wcscpy(station_name[71],L"淡水");
		wcscpy(station_name[72],L"");
		wcscpy(station_name[73],L"");
		wcscpy(station_name[74],L"");
		wcscpy(station_name[75],L"");
		wcscpy(station_name[76],L"");
		wcscpy(station_name[77],L"永寧");
		wcscpy(station_name[78],L"土城");
		wcscpy(station_name[79],L"海山");
		wcscpy(station_name[80],L"亞東醫院");
		//
		wcscpy(station_name[81],L"府中");
		wcscpy(station_name[82],L"板橋");
		wcscpy(station_name[83],L"新埔");
		wcscpy(station_name[84],L"江子翠");
		wcscpy(station_name[85],L"龍山寺");
		wcscpy(station_name[86],L"西門");
		wcscpy(station_name[87],L"");
		wcscpy(station_name[88],L"善導寺");
		wcscpy(station_name[89],L"忠孝新生(板)");
		wcscpy(station_name[90],L"忠孝復興(板)");
		//
		wcscpy(station_name[91],L"忠孝敦化");
		wcscpy(station_name[92],L"國父紀念館");
		wcscpy(station_name[93],L"市政府");
		wcscpy(station_name[94],L"永春");
		wcscpy(station_name[95],L"後山埤");
		wcscpy(station_name[96],L"昆陽");
		wcscpy(station_name[97],L"南港");
		wcscpy(station_name[98],L"南港展覽館(板)");
		wcscpy(station_name[99],L"");
		wcscpy(station_name[100],L"");
		//
		wcscpy(station_name[121],L"");
		wcscpy(station_name[122],L"");
		wcscpy(station_name[123],L"");
		wcscpy(station_name[124],L"");
		wcscpy(station_name[125],L"");
		wcscpy(station_name[126],L"");
		wcscpy(station_name[127],L"");
		wcscpy(station_name[128],L"大橋頭");
		wcscpy(station_name[129],L"民權西路(蘆)");
		wcscpy(station_name[130],L"中山國小");
		//
		wcscpy(station_name[131],L"行天宮");
		wcscpy(station_name[132],L"松江南京");
		wcscpy(station_name[133],L"忠孝新生(蘆)");
		wcscpy(station_name[134],L"");
		wcscpy(station_name[135],L"");
		wcscpy(station_name[136],L"");
		wcscpy(station_name[137],L"");
		wcscpy(station_name[138],L"");
		wcscpy(station_name[139],L"");
		wcscpy(station_name[140],L"");
		//
		wcscpy(station_name[171],L"");
		wcscpy(station_name[172],L"");
		wcscpy(station_name[173],L"");
		wcscpy(station_name[174],L"蘆洲");
		wcscpy(station_name[175],L"三民高中");
		wcscpy(station_name[176],L"徐匯中學");
		wcscpy(station_name[177],L"三和國中");
		wcscpy(station_name[178],L"三重國小");
		wcscpy(station_name[179],L"");
		wcscpy(station_name[180],L"");
		//char* station_name[] = {"","","","","","","","松山機場","中山國中","南京東路","忠孝復興(文湖線)",
		//							"大安","科技大樓","六張犁","麟光","辛亥","萬芳醫院","萬芳社區","木柵","動物園","",
		//		                    "大直","劍南路","西湖","港墘","文德","內湖","大湖公園","葫洲","東湖","南港軟體園區",
		//		                    "南港展覽館(文湖線)","小碧潭","新店","新店區公所","七張","大坪林","景美","萬隆","公館","台電大樓",
		//		                    "古亭","中正紀念堂","小南門","","頂溪","永安市場","景安","南勢角","","臺大醫院",
		//		                    "台北車站(板南線)","台北車站(淡水線)","中山","雙連","民權西路(淡水線)","圓山","劍潭","士林","芝山","明德",
		//							"石牌","唭哩岸","奇岩","北投","新北投","復興崗","忠義","關渡","竹圍","紅樹林",
		//							"淡水","","","","","","永寧","土城","海山","亞東醫院",
		//							"府中","板橋","新埔","江子翠","龍山寺","西門","","善導寺","忠孝新生(板南線)","忠孝復興(板南線)",
		//							"忠孝敦化","國父紀念館","市政府","永春","後山埤","昆陽","南港","南港展覽館(板南線)","","",
		//							"","","","","","","","","","",
		//							"","","","","","","","","","",
		//							"","","","","","","","大橋頭","民權西路(蘆洲線)","中山國小",
		//							"行天宮","松江南京","忠孝新生(蘆洲線)","","","","","","","",
		//							"","","","","","","","","","",
		//							"","","","","","","","","","",
		//							"","","","","","","","","","",
		//							"","","","蘆洲","三民高中","徐匯中學","三和國中","三重國小","",""};
	}

}

