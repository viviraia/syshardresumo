/* SisHard Resumo — navegação, tema, cópia de código, checklist, timer e quiz */
(function () {
  "use strict";

  // ---------------------------------------------------------------- navegação
  var NAV = [
    { group: "Início" },
    { href: "index.html", num: "00", label: "Visão geral e a AF" },
    { href: "cola.html", num: "★", label: "Cola para a prova" },
    { group: "2ª metade — cai na AF" },
    { href: "processos.html", num: "01", label: "Processos, fork, wait, exec" },
    { href: "entrada-saida.html", num: "02", label: "Entrada/Saída e arquivos" },
    { href: "sinais.html", num: "03", label: "Sinais" },
    { href: "threads.html", num: "04", label: "Threads e Mutex" },
    { href: "semaforos.html", num: "05", label: "Semáforos e modelos" },
    { group: "1ª metade — base (AI)" },
    { href: "parte1-assembly.html", num: "06", label: "Dados, Assembly x86-64 e gdb" },
    { group: "Treino" },
    { href: "prova.html", num: "✎", label: "Prova modelo + quiz" }
  ];

  function currentPage() {
    var p = location.pathname.split("/").pop();
    return p === "" ? "index.html" : p;
  }

  function buildSidebar() {
    var sb = document.getElementById("sidebar");
    if (!sb) return;
    var here = currentPage();
    var html = '<a class="brand" href="index.html">sis<span>hard</span>.resumo</a>' +
      '<p class="tagline">Resumo de Sistemas Hardware-Software (Insper BCC) para a Prova Final.</p>';
    NAV.forEach(function (item) {
      if (item.group) {
        html += '<div class="group">' + item.group + "</div>";
      } else {
        html += '<a class="nav' + (item.href === here ? " active" : "") + '" href="' + item.href + '">' +
          '<span class="num">' + item.num + "</span>" + item.label + "</a>";
      }
    });
    html += '<div class="tools"><button class="btn small" id="theme-btn" type="button">◐ tema</button>' +
      '<button class="btn small" type="button" onclick="window.print()">⎙ imprimir</button></div>';
    sb.innerHTML = html;

    var toggle = document.createElement("button");
    toggle.className = "btn small menu-toggle";
    toggle.type = "button";
    toggle.textContent = "☰ menu";
    toggle.addEventListener("click", function () { sb.classList.toggle("open"); });
    document.body.appendChild(toggle);
    document.addEventListener("click", function (e) {
      if (sb.classList.contains("open") && !sb.contains(e.target) && e.target !== toggle) sb.classList.remove("open");
    });
  }

  // ---------------------------------------------------------------- tema
  function initTheme() {
    var saved = null;
    try { saved = localStorage.getItem("sishard-theme"); } catch (e) {}
    if (!saved && window.matchMedia && window.matchMedia("(prefers-color-scheme: dark)").matches) saved = "dark";
    if (saved === "dark") document.documentElement.setAttribute("data-theme", "dark");
    var btn = document.getElementById("theme-btn");
    if (btn) btn.addEventListener("click", function () {
      var dark = document.documentElement.getAttribute("data-theme") === "dark";
      if (dark) document.documentElement.removeAttribute("data-theme");
      else document.documentElement.setAttribute("data-theme", "dark");
      try { localStorage.setItem("sishard-theme", dark ? "light" : "dark"); } catch (e) {}
    });
  }

  // ---------------------------------------------------------------- sumário da página
  function buildToc() {
    var toc = document.getElementById("page-toc");
    if (!toc) return;
    var hs = document.querySelectorAll("main h2[id]");
    if (!hs.length) { toc.remove(); return; }
    var html = "<strong>Nesta página</strong><ol>";
    hs.forEach(function (h) { html += '<li><a href="#' + h.id + '">' + h.textContent + "</a></li>"; });
    toc.innerHTML = html + "</ol>";
  }

  // ---------------------------------------------------------------- realce de sintaxe C (simples)
  var KW = "auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while|pid_t|pthread_t|pthread_mutex_t|sem_t|ssize_t|size_t|NULL|#include|#define";
  var TOKEN = new RegExp(
    "(\\/\\/[^\\n]*|\\/\\*[\\s\\S]*?\\*\\/)" +          // 1 comentário
    "|(\"(?:\\\\.|[^\"\\\\\\n])*\"|'(?:\\\\.|[^'\\\\\\n])')" + // 2 string/char
    "|(^|[^\\w#])(" + KW + ")(?![\\w])" +               // 3,4 keyword
    "|([A-Za-z_]\\w*)(?=\\s*\\()",                     // 5 chamada
    "gm");
  function esc(s) { return s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;"); }
  function highlight(code) {
    var out = "", last = 0, m;
    var src = code.textContent;
    TOKEN.lastIndex = 0;
    while ((m = TOKEN.exec(src)) !== null) {
      out += esc(src.slice(last, m.index));
      if (m[1]) out += '<span class="c">' + esc(m[1]) + "</span>";
      else if (m[2]) out += '<span class="s">' + esc(m[2]) + "</span>";
      else if (m[4]) out += esc(m[3]) + '<span class="k">' + esc(m[4]) + "</span>";
      else if (m[5]) out += '<span class="f">' + esc(m[5]) + "</span>";
      last = TOKEN.lastIndex;
      if (m[0].length === 0) TOKEN.lastIndex++;
    }
    out += esc(src.slice(last));
    code.innerHTML = out;
  }

  function initCode() {
    document.querySelectorAll("pre").forEach(function (pre) {
      var code = pre.querySelector("code") || pre;
      if (pre.getAttribute("data-lang") === "c") highlight(code);
      var btn = document.createElement("button");
      btn.className = "copy-btn";
      btn.type = "button";
      btn.textContent = "copiar";
      btn.addEventListener("click", function () {
        var text = code.textContent;
        var done = function () { btn.textContent = "copiado!"; setTimeout(function () { btn.textContent = "copiar"; }, 1300); };
        if (navigator.clipboard && navigator.clipboard.writeText) {
          navigator.clipboard.writeText(text).then(done, function () { fallbackCopy(text); done(); });
        } else { fallbackCopy(text); done(); }
      });
      pre.appendChild(btn);
    });
  }
  function fallbackCopy(text) {
    var ta = document.createElement("textarea");
    ta.value = text; document.body.appendChild(ta); ta.select();
    try { document.execCommand("copy"); } catch (e) {}
    ta.remove();
  }

  // ---------------------------------------------------------------- checklist persistente
  function initChecklist() {
    var lists = document.querySelectorAll(".checklist[data-key]");
    lists.forEach(function (ul) {
      var key = "sishard-check-" + ul.getAttribute("data-key");
      var state = {};
      try { state = JSON.parse(localStorage.getItem(key) || "{}"); } catch (e) {}
      var boxes = ul.querySelectorAll("input[type=checkbox]");
      var bar = document.querySelector('.progress[data-for="' + ul.getAttribute("data-key") + '"] > div');
      function update() {
        var n = 0;
        boxes.forEach(function (b) { if (b.checked) n++; });
        if (bar) bar.style.width = (100 * n / boxes.length) + "%";
      }
      boxes.forEach(function (b, i) {
        var id = key + "-" + i;
        b.id = id;
        var lab = b.nextElementSibling;
        if (lab && lab.tagName === "LABEL") lab.setAttribute("for", id);
        b.checked = !!state[i];
        b.addEventListener("change", function () {
          state[i] = b.checked;
          try { localStorage.setItem(key, JSON.stringify(state)); } catch (e) {}
          update();
        });
      });
      update();
    });
  }

  // ---------------------------------------------------------------- timer de prova (3h)
  function initTimer() {
    var el = document.getElementById("timer");
    if (!el) return;
    var TOTAL = 3 * 60 * 60;
    var KEY = "sishard-timer";
    var st = { remaining: TOTAL, running: false, t0: 0 };
    try { st = JSON.parse(localStorage.getItem(KEY)) || st; } catch (e) {}
    var clock = el.querySelector(".clock");
    var startBtn = el.querySelector("[data-act=start]");
    var resetBtn = el.querySelector("[data-act=reset]");

    function left() {
      if (!st.running) return st.remaining;
      return Math.max(0, st.remaining - Math.floor((Date.now() - st.t0) / 1000));
    }
    function save() { try { localStorage.setItem(KEY, JSON.stringify(st)); } catch (e) {} }
    function fmt(s) {
      var h = Math.floor(s / 3600), m = Math.floor((s % 3600) / 60), x = s % 60;
      return h + ":" + (m < 10 ? "0" : "") + m + ":" + (x < 10 ? "0" : "") + x;
    }
    function render() {
      var s = left();
      clock.textContent = fmt(s);
      clock.classList.toggle("warn", s <= 15 * 60);
      startBtn.textContent = st.running ? "❚❚ pausar" : (s === TOTAL ? "▶ iniciar prova" : "▶ continuar");
      if (st.running && s === 0) { st.running = false; st.remaining = 0; save(); clock.textContent = "TEMPO!"; }
    }
    startBtn.addEventListener("click", function () {
      if (st.running) { st.remaining = left(); st.running = false; }
      else { st.t0 = Date.now(); st.running = true; }
      save(); render();
    });
    resetBtn.addEventListener("click", function () {
      if (!confirm("Zerar o cronômetro para 3:00:00?")) return;
      st = { remaining: TOTAL, running: false, t0: 0 }; save(); render();
    });
    render();
    setInterval(render, 1000);
  }

  // ---------------------------------------------------------------- quiz
  function initQuiz() {
    var root = document.getElementById("quiz");
    if (!root || !window.QUIZ) return;
    var KEY = "sishard-quiz";
    var answers = {};
    try { answers = JSON.parse(localStorage.getItem(KEY) || "{}"); } catch (e) {}
    var filter = "todas";
    var graded = false;
    var LETTERS = "abcdefgh";

    var controls = document.createElement("div");
    controls.className = "quiz-filter";
    controls.innerHTML =
      '<button class="btn small" data-f="todas" type="button">Todas (' + QUIZ.length + ")</button>" +
      '<button class="btn small" data-f="af" type="button">Só AF (' + QUIZ.filter(function (q) { return q.part === "af"; }).length + ")</button>" +
      '<button class="btn small" data-f="ai" type="button">Só Assembly/AI (' + QUIZ.filter(function (q) { return q.part === "ai"; }).length + ")</button>";
    root.appendChild(controls);

    var list = document.createElement("div");
    root.appendChild(list);

    var actions = document.createElement("div");
    actions.className = "files";
    actions.innerHTML = '<button class="btn primary" id="quiz-grade" type="button">✓ Corrigir quiz</button>' +
      '<button class="btn" id="quiz-reset" type="button">↺ Refazer (limpar respostas)</button>';
    root.appendChild(actions);

    var score = document.createElement("div");
    score.className = "quiz-score";
    score.id = "quiz-score";
    root.appendChild(score);

    function visible() {
      return QUIZ.filter(function (q) { return filter === "todas" || q.part === filter; });
    }

    function render() {
      list.innerHTML = "";
      visible().forEach(function (q, idx) {
        var div = document.createElement("div");
        div.className = "quiz-q" + (graded ? " graded" : "");
        div.setAttribute("data-id", q.id);
        var html = '<span class="qnum">Questão ' + (idx + 1) + "</span>" +
          '<span class="qtopic">' + q.topic + "</span>" +
          '<div class="qtext">' + q.q + '</div><div class="quiz-opts">';
        q.opts.forEach(function (o, i) {
          var cls = "quiz-opt";
          if (answers[q.id] === i) cls += " selected";
          if (graded) {
            if (i === q.ans) cls += " correct";
            else if (answers[q.id] === i) cls += " wrong";
          }
          html += '<button type="button" class="' + cls + '" data-i="' + i + '"' + (graded ? " disabled" : "") + ">" +
            '<span class="letter">' + LETTERS[i] + ")</span><span>" + o + "</span></button>";
        });
        html += '</div><div class="quiz-expl"><strong>' +
          (answers[q.id] === q.ans ? "✔ Acertou." : (answers[q.id] === undefined ? "— Em branco." : "✘ Errou.")) +
          "</strong> Resposta: " + LETTERS[q.ans] + ") " + q.expl + "</div>";
        div.innerHTML = html;
        list.appendChild(div);
      });
      controls.querySelectorAll("button").forEach(function (b) {
        b.classList.toggle("primary", b.getAttribute("data-f") === filter);
      });
      if (window.__highlightNew) window.__highlightNew(list);
    }

    list.addEventListener("click", function (e) {
      var opt = e.target.closest(".quiz-opt");
      if (!opt || graded) return;
      var id = opt.closest(".quiz-q").getAttribute("data-id");
      answers[id] = parseInt(opt.getAttribute("data-i"), 10);
      try { localStorage.setItem(KEY, JSON.stringify(answers)); } catch (err) {}
      opt.parentNode.querySelectorAll(".quiz-opt").forEach(function (b) { b.classList.remove("selected"); });
      opt.classList.add("selected");
    });

    controls.addEventListener("click", function (e) {
      var b = e.target.closest("button");
      if (!b) return;
      filter = b.getAttribute("data-f");
      graded = false;
      score.classList.remove("show");
      render();
    });

    document.getElementById("quiz-grade").addEventListener("click", function () {
      graded = true;
      render();
      var qs = visible(), ok = 0, blank = 0, topics = {};
      qs.forEach(function (q) {
        var t = topics[q.topic] || (topics[q.topic] = { ok: 0, n: 0 });
        t.n++;
        if (answers[q.id] === q.ans) { ok++; t.ok++; }
        else if (answers[q.id] === undefined) blank++;
      });
      var nota = (10 * ok / qs.length).toFixed(1);
      var html = '<div class="big">' + ok + " / " + qs.length + " &nbsp;→ nota " + nota + "</div>" +
        "<div>" + blank + " em branco. Por tópico:</div><ul>";
      Object.keys(topics).forEach(function (k) {
        html += "<li>" + k + ": " + topics[k].ok + "/" + topics[k].n + "</li>";
      });
      html += "</ul><div>Revise as explicações abaixo de cada questão errada.</div>";
      score.innerHTML = html;
      score.classList.add("show");
      score.scrollIntoView({ behavior: "smooth", block: "center" });
    });

    document.getElementById("quiz-reset").addEventListener("click", function () {
      answers = {};
      graded = false;
      try { localStorage.removeItem(KEY); } catch (e) {}
      score.classList.remove("show");
      render();
    });

    render();
  }

  window.__highlightNew = function (scope) {
    scope.querySelectorAll('pre[data-lang="c"]').forEach(function (pre) {
      if (!pre.__hl) { highlight(pre.querySelector("code") || pre); pre.__hl = true; }
    });
  };

  document.addEventListener("DOMContentLoaded", function () {
    buildSidebar();
    initTheme();
    buildToc();
    initCode();
    document.querySelectorAll('pre[data-lang="c"]').forEach(function (p) { p.__hl = true; });
    initChecklist();
    initTimer();
    initQuiz();
  });
})();
