# 爬虫库requests
import requests
import asyncio
import aiohttp
import aiofiles
import json

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
# 构建会话session,并使所有请求及其回应都发生在一个会话中
url = 'http://www.baidu.com'
user_agent = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.139 Safari/537.36'
with requests.session() as session:
    # 会话支持POST和GET请求
    # with session.post(url, headers={'User-Agent': user_agent}) as response:
    with session.get(url, headers={'User-Agent': user_agent}) as response:
        assert response.status_code == 200
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


'''
# 异步协程获取西游记(因为速度过快,服务器会掐断连接,待定中)

async def aio_download(index, cid, title, b_id):
    url = 'https://dushu.baidu.com/api/pc/getChapterContent?data='
    chapter_url = url + json.dumps({"book_id": b_id, "cid": f'{b_id}|{cid}', "need_bookinfo": 0})
    print(chapter_url)
    async with aiohttp.ClientSession() as session:
        async with session.get(chapter_url) as response:
            async with aiofiles.open('西游记/%4d%s' % (index, title), 'wb') as file:
                await file.write(json.loads(await response.text())['data']['novel']['content'])


async def main():
    book_id = '4306063500'
    bool_url = 'https://dushu.baidu.com/api/pc/getCatalog?data={"book_id":"' + book_id + '"}'
    with requests.get(bool_url) as response:
        assert response.status_code == 200
        response.encoding = 'utf-8'
        await asyncio.wait([asyncio.create_task(aio_download(index, item['cid'], item['title'], book_id))
                            for index, item in enumerate(response.json()['data']['novel']['items'])])

if __name__ == '__main__':
    asyncio.run(main())
'''


'''
# 下载91看剧资源参考案例,电影和电视流程一致
import requests
import asyncio
import aiohttp
import aiofiles
import functools
import re
import os
import shutil
from Crypto.Cipher import AES


def ts_name_compare(ts_name1, ts_name2):
    match1 = re.findall('./spider_dl_tt_temp/(.*?).ts', ts_name1)
    match2 = re.findall('./spider_dl_tt_temp/(.*?).ts', ts_name2)
    if int(match1[0]) < int(match2[0]):
        return -1
    if int(match1[0]) > int(match2[0]):
        return +1
    return 0


async def frame_download_and_dump(index, url, key_value, session):
    aes = AES.new(key=key_value, IV=b"0000000000000000", mode=AES.MODE_CBC)
    async with session.get(url) as response:
        assert response.status == 200
        response.encoding = 'utf-8'
        async with aiofiles.open('./spider_dl_tt_temp/%d.ts' % index, mode='wb') as file:
            await file.write(aes.decrypt(await response.content.read()))


async def main():
    # Step1: 从起始源获得页面
    url = 'https://91kju.com/vod-play-id-8482-sid-1-pid-1.html'     # 杀戒(2022-07-25资源不稳定)
    # url = 'https://91kju.com/vod-play-id-63237-sid-1-pid-1.html'  # 齐天大圣(2022-07-25资源不稳定)
    with requests.get(url) as response:
        assert response.status_code == 200
        response.encoding = 'utf-8'
        # 提取转接头
        adaptor_url = re.findall('cms_player(.*?)"url":"(.*?)"', str(response.text))[0][1].replace('\\', '')
        print(adaptor_url)
    with requests.get(adaptor_url) as response:
        assert response.status_code == 200
        response.encoding = 'utf-8'
        # 提取第一级m3u8
        m3u8_url = response.url
        print(m3u8_url)
    # Step2: 从一级界面提取二级界面
    with requests.get(m3u8_url) as response:
        assert response.status_code == 200
        response.encoding = 'utf-8'
        # 提取第二级m3u8
        m3u8_url = re.findall('(.*?)/[0-9](.&?)', m3u8_url)[0][0] + str(response.text).split('\n')[2]
        print(m3u8_url)
    # Step3: 从二级界面提取Key界面和jpg_list界面
    with requests.get(m3u8_url) as response:
        assert response.status_code == 200
        response.encoding = 'utf-8'
        # 提取key
        key_url = re.findall('URI="(.*?)"(.*?)', response.text)[0][0]
        print(key_url)
        # 提取ts列表,这里都是.jpg格式的文件,需要移除开头的一部分格式头
        jpg_url_lst = [(index, http) for index, http in enumerate(re.findall(',\n(.*?)\n', response.text))]
        # print(jpg_url_lst)
    # Step4: 从Key界面提取加密秘钥
    with requests.get(key_url) as response:
        assert response.status_code == 200
        response.encoding = 'utf-8'
        # 提取key值
        key_value = response.content
        print(key_value)
    # Step5: 创建一个相对路径文件夹,如果它不存在的话,否则先移除再创建
    if os.path.isdir('./spider_dl_tt_temp'):
        shutil.rmtree('./spider_dl_tt_temp')
    if not os.path.isdir('./spider_dl_tt_temp'):
        os.mkdir('./spider_dl_tt_temp')
    # Step6: 创建异步协程任务集并且等待任务集执行完毕
    async with aiohttp.ClientSession() as session:
        await asyncio.wait([asyncio.create_task(frame_download_and_dump(item[0], item[1], key_value, session))
                            for item in jpg_url_lst])
    # Step7: 从名字切取特质标识,提取临时文件,并进行组包,注意要对其排序,否则为错误的顺序
    ts_list = ['./spider_dl_tt_temp/%d.ts' % item[0] for item in jpg_url_lst]
    ts_list.sort(key=functools.cmp_to_key(ts_name_compare))
    # 创建目标文件,如果它不存在的话
    if not os.path.isfile(' ./%s.mp4' % url.rsplit('/', 1)[1].split('.')[0]):
        open(' ./%s.mp4' % url.rsplit('/', 1)[1].split('.')[0], mode='wb').close()
    os.system('copy /b ' + ' '.join(ts_list) + ' ./%s.mp4' % url.rsplit('/', 1)[1].split('.')[0])
    print('copy /b ' + ' '.join(ts_list) + ' ./%s.mp4' % url.rsplit('/', 1)[1].split('.')[0])
    # Step x: 移除临时文件夹及其临时文件
    if os.path.isdir('./spider_dl_tt_temp'):
        shutil.rmtree('./spider_dl_tt_temp')
    print('--over--')

if __name__ == '__main__':
    asyncio.run(main())
'''
