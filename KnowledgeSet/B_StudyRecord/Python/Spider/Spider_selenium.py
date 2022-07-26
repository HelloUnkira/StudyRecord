# selenium爬取招聘网站信息
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.select import Select
from selenium.webdriver import Firefox
from selenium.webdriver.firefox.options import Options
import time

'''
# 使用火狐并开启一个浏览器
browser = Firefox()
browser.get('https://www.zhaopin.com/citymap')
# 模拟回车键出现意外,暂未知其情况
# city = browser.find_element(by=By.CLASS_NAME, value='city-search__box__text--field')
# city.send_keys('长沙')
# city.send_keys(Keys.ENTER)
browser.find_element(by=By.XPATH, value='/html/body/div/div[2]/div[3]/div[3]/ul/li[5]/a').click()
# 切换到新打开的那一个窗口(最后一个窗口)
# print(browser.window_handles)
time.sleep(5)
browser.switch_to.window(browser.window_handles[-1])
browser.find_element(by=By.CLASS_NAME, value='zp-search__input').send_keys('嵌入式软件工程师')
browser.find_element(by=By.CLASS_NAME, value='zp-search__btn').click()
# 切换到新打开的那一个窗口(最后一个窗口)
# print(browser.window_handles)
time.sleep(5)
browser.switch_to.window(browser.window_handles[-1])
browser.find_element(by=By.CLASS_NAME, value='query-search__content-input').send_keys('嵌入式软件工程师')
browser.find_element(by=By.CLASS_NAME, value='query-search__content-button').click()
# 获取当前页有效信息(简要获取)
time.sleep(5)
job_lst = browser.find_elements(by=By.CLASS_NAME, value='joblist-box__item')
for item in job_lst:
    try:
        job_name = item.find_element(by=By.CLASS_NAME, value='iteminfo__line1__jobname').text
        job_company = item.find_element(by=By.CLASS_NAME, value='iteminfo__line1__compname').text
        job_salary = item.find_element(by=By.CLASS_NAME, value='iteminfo__line2__jobdesc__salary').text
        job_company_size = item.find_elements(by=By.CLASS_NAME, value='iteminfo__line2__compdesc__item')[1].text
        job_digest = [i.text for i in item.find_elements(by=By.CLASS_NAME, value='iteminfo__line3__welfare__item')]
        print('----------------------')
        job_digest_info = [job_name, job_company, job_salary, job_company_size, job_digest]
        print(job_digest_info)
        print('----------------------')
    except Exception as e:
        continue
'''
