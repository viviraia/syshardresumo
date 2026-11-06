// Gera cola.pdf a partir de cola.html (layout de impressão, A4).
// Uso (na raiz do repositório): node scripts/gerar-pdf-cola.js
// Requer Playwright com Chromium: npm i -D playwright && npx playwright install chromium
const path = require('path');
const { chromium } = require('playwright');

(async () => {
  const raiz = path.resolve(__dirname, '..');
  const browser = await chromium.launch();
  const page = await browser.newPage();
  await page.goto('file://' + path.join(raiz, 'cola.html'), { waitUntil: 'load' });
  await page.emulateMedia({ media: 'print', colorScheme: 'light' });
  await page.pdf({
    path: path.join(raiz, 'cola.pdf'),
    format: 'A4',
    printBackground: true,
    preferCSSPageSize: true,
    displayHeaderFooter: true,
    headerTemplate: '<div style="font-size:8px;width:100%;text-align:center;color:#777">SisHard · Cola para a Prova Final</div>',
    footerTemplate: '<div style="font-size:8px;width:100%;text-align:center;color:#777"><span class="pageNumber"></span> / <span class="totalPages"></span></div>',
  });
  await browser.close();
  console.log('cola.pdf gerado');
})();
