document.addEventListener('DOMContentLoaded', () => {
  window.addEventListener('resize', handleResize);
  handleResize();

  function handleResize() {
    const sidebarNotFixed = document.getElementById('sidebar_notFixed');
    const sidebarFixed = document.getElementById('sidebar_fixed');
    const elems = document.querySelectorAll('#sidebar [data-sticky]');
    const winHeight = window.innerHeight;

    // data-sticky 属性を持つ要素を後ろから見ていき、
    // ウィンドウ内に収まる要素の data-sticky 属性の値を true にする。
    // NodeList は配列ではないので、Array.from(elems) で配列に変換してから reverse() する。
    let sum = 0;
    Array.from(elems).reverse().forEach((elem) => {
      const style = getComputedStyle(elem);
      sum += elem.offsetHeight + parseInt(style.marginTop) + parseInt(style.marginBottom);
      elem.setAttribute('data-sticky', sum < winHeight ? 'true' : 'false');
    });

    // サイドバー内の要素を #sidebar_notFixed と #sidebar-fixed の子要素として振り分ける。
    // 順番がおかしくならないようにループを分ける。
    elems.forEach((elem) => {
      if (elem.getAttribute('data-sticky') === 'true') {
        sidebarFixed.appendChild(elem);
      } else {
        sidebarNotFixed.appendChild(elem);
      }
    });
  }
});

