# 爬虫库requests
import requests
from lxml import etree
from urllib.parse import quote
import os
import re
import time
import unicodedata

'''
url = 'http://www.baidu.com'
user_agent = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36'
proxy = '221.4.241.198:9091'
# 1.requests.request请求
# response = requests.request('get', url, params={'from': 'navigation'})
# 2.requests.post请求
# response = requests.post(url, params={'type': 5, 'interval_id': '100:90', 'action': ''}, data={'start': 1, 'limit': 20},
#                          headers={'User-Agent': user_agent})
# 3.requests.get请求
response = requests.get(url, headers={'User-Agent': user_agent}, proxies={'http': proxy})
# ......
assert response.status_code == 200
response.encoding = 'utf-8'
with open('index.html', 'wb') as file:
    file.write(response.content)
'''


'''
# 古诗文下载
page = 0
txt_name = 'classical_chinese_poetry.txt'
base_url = 'https://www.gushiwen.cn/default_%s.aspx'
user_agent = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36'
if os.path.isfile(txt_name):
    os.remove(txt_name)
# 文本清洗:全角字符转半角字符
C_pun = u'〔〕（），。！？【】（）《》“‘：；［］｛｝&，．？＼％－＋￣~＄#＠=＿、／'
E_pun = u'()(),.!?[]()<>"\':;[]{}&,.?\\%-+~~$#@=_//'
table = {ord(f): ord(t) for f, t in zip(C_pun, E_pun)}
while True:
    page += 1   # 从第一页开始
    url = base_url % page
    response = requests.get(url, headers={'User-Agent': user_agent})
    assert response.status_code == 200
    response.encoding = 'utf-8'
    root = etree.HTML(response.text)
    new_url = root.xpath('//a[@id="amore"]/@href')
    divs = root.xpath('//div[@class="left"]/div[@class="sons"]')
    for div in divs:
        name = div.xpath('.//p[1]//text()')
        author = ' '.join(div.xpath('.//p[2]/a/text()'))
        content = re.sub('[ \r\n]', '', ' '.join(div.xpath('.//div[@class="contson"]/text()')))
        if not (name and author and content):
            continue
        item = {'name': name[0], 'author': author, 'content': content}
        string = str(item).translate(table)
        with open(txt_name, 'a', encoding='utf-8') as file:
            file.write(string + '\r\n')
    print('download page %s succeed' % page)
    if not new_url:
        break
    time.sleep(1)
'''


'''
# 图片下载(美女桌面壁纸)
# index, index_1, ...
website = 'http://www.netbian.com'
base_url = 'http://www.netbian.com/meinv/index%s.htm'
user_agent = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.71 Safari/537.36 Core/1.94.169.400 QQBrowser/11.0.5130.400'
for page in range(0, 64):
    page += 1
    if page == 1:
        url = base_url % ''
    else:
        url = base_url % '_%s' % page
    response = requests.get(url, headers={'User-Agent': user_agent})
    assert response.status_code == 200
    response.encoding = 'utf-8'
    ul = etree.HTML(response.text).xpath('//div[@id="main"]//div[@class="list"]/ul')
    for li in ul:
        for index in range(1, 20):
            if not li.xpath('./li[%s]/a/img/@src' % index):
                continue
            url1 = website + li.xpath('./li[%s]/a/@href' % index)[0]
            print('url1:\t', url1)
            response1 = requests.get(url1, headers={'User-Agent': user_agent})
            assert response1.status_code == 200
            response1.encoding = 'utf-8'
            url2 = etree.HTML(response1.text).xpath('//div[@id="main"]//div[@class="pic"]/p/a/img/@src')[0]
            response1.close()
            print('url2:\t', url2)
            response2 = requests.get(url2, headers={'User-Agent': user_agent})
            assert response2.status_code == 200
            response2.encoding = 'utf-8'
            with open('img/meinv_%d_%d.jpg' % (page, index), 'wb') as file:
                file.write(response2.content)
            response2.close()
            time.sleep(0.5)
    print('download page %d success' % page)
    time.sleep(1)
response.close()
'''
