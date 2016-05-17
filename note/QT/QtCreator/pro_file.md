1.加快编译速度
	使用多线程 
	QMAKE_CXXFLAGS += /MP
	开ccache
	QMAKE_CC = ccache $$QMAKE_CC
	QMAKE_CXX = ccache $$QMAKE_CXX
	//只找到了最后一个方法
	项目-》构建和运行-》构建步骤-》make详情-》make参数 ，填入-j4
