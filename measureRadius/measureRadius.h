// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� MEASURERADIUS_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// MEASURERADIUS_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
/*#ifdef MEASURERADIUS_EXPORTS
#define MEASURERADIUS_API __declspec(dllexport)
#else
#define MEASURERADIUS_API __declspec(dllimport)
#endif*/

/*
// �����Ǵ� measureRadius.dll ������
class MEASURERADIUS_API CmeasureRadius {
public:
	CmeasureRadius(void);
	// TODO:  �ڴ�������ķ�����
};

extern MEASURERADIUS_API int nmeasureRadius;

MEASURERADIUS_API int fnmeasureRadius(void);*/

extern "C" _declspec(dllexport) float* getWJ();
extern "C" _declspec(dllexport) float getNJ();

