#include "../Includes/Client.hpp"

void	Client::store_type(void)
{
	mime_type["text/html"] = ".html";
	mime_type["text/css"] = ".css";
	mime_type["text/xml"] = ".rss";
	mime_type["image/gif"] = ".gif";
	mime_type["image/jpeg"] = ".jpeg";
	mime_type["text/plain"] = ".txt";
	mime_type["image/png"] = ".png";
	mime_type["image/x-icon"] = ".ico";
	mime_type["image/x-jng"] = ".jng";
	mime_type["application/pdf"] = ".pdf";	
	mime_type["audio/mpeg"] = ".mp3";
	mime_type["video/mp4"] = ".mp4";
	mime_type["application/octet-stream"] = ".py";
	mime_type["text/html"] = ".php";
}


