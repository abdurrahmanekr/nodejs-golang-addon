# Node.js'e Go Eklentisi Yazmak

Bu proje [Node.js, Golang ve Electron.js Kullanarak Performanslı Masaüstü Uygulamalar Geliştirmek](https://avarekodcu.com/konu/55/nodejs-golang-ve-electronjs-kullanarak-performansli-masaustu-uygulamalar-gelistirmek) başlığı altında yayınlanmıştır.

Projenin amacı Go'nun yüksek CPU kullanımındaki avantajını Node.js ile birleştirerek resim işleme, video işleme, makine öğrenmesi ve aklınıza gelebilecek her uygulamada kullanmaktır.

Electron'da masaüstü uygulaması yapmak istiyorsunuz ve bu uygulama offline şekilde CPU kullanımı yüksek bir iş yapacak diyelim. Bu durumda Node.js eklentisi yazarak çözüm bulabilirsiniz. Ancak C++'ın zorluğuyla uğraşmanız gerekecek. Bu projede buna gerek kalmadan sadece bir köprü kodla Go yazarak istediğinizi yüksek CPU kullanımına sahip bir uygulama yapabilirsiniz. Üstelik web servislerle ve JSON ile uğraşmaya gerek kalmadan direkt JS içerisinde kullanabiliyorsunuz.

Örneğin anlaşılması için:

```js
const HighCPUModule = require('./build/Release/addon');

HighCPUModule.cutImage(
    '/path/to/file',
    [400, 500],
    (loading) => console.log(loading + '% of the process is done.'),
)
.then((res) => console.log('Process done. result path: ' + res))
```