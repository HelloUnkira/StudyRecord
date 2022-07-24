# 爬虫库requests
import requests
from lxml import etree
import time

# 不使用异步协程, 以降低被封锁IP的可能性, 因为这里没使用到代理
# 2022-07-24: 非4K子页面当前只需要提供User-Agent
# 美女桌面壁纸网站(非4K壁纸, 4K壁纸资源已经下架, 4K子页面需额外提供Referer和Cookie(动态), 操作流程一致, 路径微修):
user_agent = 'Mozilla/5.0 (Windows NT 10.0; WOW64)' \
             'AppleWebKit/537.36 (KHTML, like Gecko) ' \
             'Chrome/94.0.4606.71 Safari/537.36 Core/1.94.169.400 QQBrowser/11.0.5130.400'
website = 'http://www.netbian.com'
url0 = website + '/meinv/index%s.htm'
# 图片下载(美女桌面壁纸),页面索引: index, index_1, ...
for page, p_url in [(page, url0 % '_%s' % page if page != 1 else url0 % '') for page in range(1, 65)]:
    with requests.get(p_url, headers={'User-Agent': user_agent}) as response0:
        assert response0.status_code == 200
        response0.encoding = 'utf-8'
        for li in etree.HTML(response0.text).xpath('//div[@id="main"]//div[@class="list"]/ul'):
            for index, url1 in [(index, website + li.xpath('./li[%s]/a/@href' % index)[0]) for index in range(1, 21)
                                if li.xpath('./li[%s]/a/img/@src' % index)]:
                print('url1:\t', url1)
                with requests.get(url1, headers={'User-Agent': user_agent}) as response1:
                    assert response1.status_code == 200
                    response1.encoding = 'utf-8'
                    url2 = etree.HTML(response1.text).xpath('//div[@id="main"]//div[@class="pic"]/p/a/img/@src')[0]
                print('url2:\t', url2)
                with requests.get(url2, headers={'User-Agent': user_agent}) as response2:
                    # 可能存在某些图片源丢失,直接跳过即可
                    if not response2.status_code == 200:
                        break
                    response2.encoding = 'utf-8'
                    with open('img/meinv_%d_%d.jpg' % (page, index), 'wb') as file:
                        file.write(response2.content)
                time.sleep(0.1)     # 备注:为了保证稳定的下载, 可能还需要适当的增加图片的下载时间, 以减少对服务器的负载
        print('download page %d success' % page)
        time.sleep(1)               # 备注:为了保证稳定的下载, 可能还需要适当的增加图片的下载时间, 以减少对服务器的负载
