#pragma once
#include <iostream>
#include <filesystem>
#include "Singleton.h"

namespace why
{
	/**
	* @author	why
	* @brief 为当前路径拼接构建路径
	* @param strUTF8 文件的全路径名(UTF-8编码)
	* @return 返回路径对象
	*/
	std::string AppendBuildPath(const std::string& path);

	/**
	* @author	why
	* @brief 为当前路径拼接构建路径
	* @param strUTF8 文件的全路径名(UTF-8编码)
	* @return 返回路径对象
	*/
	std::string GetExecutableDirectory();

	/**
	* @author	why
	* @brief	链式编程为源路径添加子路径
	*/
	class PathAppender
	{
	public:
		template<typename> friend class Singleton;
		std::string GetPath() { return m_strPath; };
		
		PathAppender& AppendChildPath(const std::string& strChildPath)
		{
			m_strPath += std::filesystem::path::preferred_separator;
			m_strPath += strChildPath;
			return *this;
		}
		PathAppender& SetSourcePath(const std::string& strSourcePath)
		{
			Clear();
			m_strPath = strSourcePath;
			return *this;
		}
		void Clear() { m_strPath = ""; };
	private:
		PathAppender() {};
		std::string m_strPath;
	};


	/**
	* @brief 从UTF8编码的文件名获取文件路径对象
	* @param strUTF8 文件的全路径名(UTF-8编码)
	* @return 返回路径对象
	*/
	std::filesystem::path UTF8ToPath(const std::string& strUTF8);

	/**
	* @brief 从文件对象获取UTF-8编码的文件名称
	* @param strUTF8 文件的全路径名(UTF-8编码)
	* @return 返回路径对象
	*/
	std::string PathToUTF8(const std::filesystem::path& filePath);

	/**
* @brief 创建目录
* @param dir 待创建得目录名称
* @return
* @ - 0 创建成功
* @ - 其他 创建失败
* @warning
* 在windows平台上，使用GB2312编码
* 在非windwos平台上，使用UTF-8编码
*/
	bool CreateDirectories(const std::string& dir);
}