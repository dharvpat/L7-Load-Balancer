// frontend_flask/app/static/js/scripts.js

// Example: Function to add more server fields dynamically
document.addEventListener('DOMContentLoaded', function() {
    const addServerBtn = document.getElementById('add-server-btn');
    if (addServerBtn) {
        addServerBtn.addEventListener('click', function(e) {
            e.preventDefault();
            const serversDiv = document.querySelector('.servers');
            const newServerDiv = document.createElement('div');
            newServerDiv.classList.add('server');
            newServerDiv.innerHTML = `
                <input type="text" name="server_id" placeholder="Server ID" required>
                <input type="text" name="server_ip" placeholder="Server IP" required>
                <input type="number" name="server_port" placeholder="Port" required>
                <input type="number" name="server_weight" placeholder="Weight" required>
            `;
            serversDiv.appendChild(newServerDiv);
        });
    }
});