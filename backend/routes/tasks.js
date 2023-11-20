var express = require("express");
var router = express.Router();

let tasks = [];

router.get("/", function (req, res, next) {
  res.json(tasks);
});

router.post("/", function (req, res, next) {
  const taskName = req.body.name; // Nome da tarefa
  const taskText = req.body.text; // Texto da tarefa

  console.log(req.body);

  if (!taskName || !taskText) {
    return res
      .status(400)
      .json({ error: "Nome e texto da tarefa são obrigatórios." });
  }

  const newTask = {
    id: tasks.length + 1,
    name: taskName,
    text: taskText,
  };

  tasks.push(newTask);

  res.status(201).json(newTask);
});

module.exports = router;
