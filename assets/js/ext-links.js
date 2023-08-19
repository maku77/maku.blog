document.addEventListener('DOMContentLoaded', () => {
  const links = document.querySelectorAll('a[href^=http]');
  for (let i = 0; i < links.length; ++i) {
    const link = links[i];

    // 子に img 要素を持つ場合はアイコンを表示しない
    const images = link.getElementsByTagName('img');
    if (images.length > 0) { continue; }

    // 子に svg 要素を持つ場合はアイコンを表示しない
    const svgs = link.getElementsByTagName('svg');
    if (svgs.length > 0) { continue; }

    // クラスを追加して外部リンクアイコンを表示
    link.classList.add('xExternalLinkIcon');

    // 外部リンクを必ず新しいタブで開く
    // （rel=noopener を指定することにより、必ず別プロセスで開く）
    link.setAttribute('target', '_blank');
    link.setAttribute('rel', 'noopener');
  }
});
